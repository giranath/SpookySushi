#include "game_loop.hpp"
#include "window_config.hpp"
#include "window.hpp"
#include "display.hpp"
#include "opengl.hpp"
#include "sdl_event.hpp"
#include "../../rendering/public/renderer_interface.hpp"
#include "../../rendering/public/opengl_renderer.hpp"
#include "../../rendering/public/proxy_renderer.hpp"
#include <static_mesh_builder_service.hpp>
#include <debug_draw_service.hpp>
#include <input_bus_service.hpp>
#include "../../input/public/input_factory.hpp"
#include "../../input/public/input_event.hpp"
#include "../../input/public/input_bus.hpp"
#include "../../service/public/service_locator.hpp"
#include "../../input/public/input_processor.hpp"

#include <SDL.h>

namespace sushi {

void GameLoop::setup_sdl() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("cannot initialize SDL");
    }

    SDL_LogSetOutputFunction(sdl_log_output, nullptr);
}

void GameLoop::setup_jobs(const JobConfig& configs) {
    jobs_ = std::make_unique<sushi::async::engine>(configs.worker_count, configs.max_job_count);
}

GameLoop::GameLoop(const GameLoopConfig& configs)
: jobs_(nullptr) {
    setup_sdl();
    setup_jobs(configs.jobs);
}

GameLoop::~GameLoop() noexcept {
    SDL_Quit();
}

void sdl_log_output(void* userdata, int category, SDL_LogPriority priority, const char* message) {
    std::string cat = "org.sdl.";
    switch(category) {
        case SDL_LOG_CATEGORY_APPLICATION:
            cat += "application";
            break;
        case SDL_LOG_CATEGORY_ERROR:
            cat += "error";
            break;
        case SDL_LOG_CATEGORY_ASSERT:
            cat += "assert";
            break;
        case SDL_LOG_CATEGORY_SYSTEM:
            cat += "system";
            break;
        case SDL_LOG_CATEGORY_AUDIO:
            cat += "audio";
            break;
        case SDL_LOG_CATEGORY_VIDEO:
            cat += "video";
            break;
        case SDL_LOG_CATEGORY_RENDER:
            cat += "render";
            break;
        case SDL_LOG_CATEGORY_INPUT:
            cat += "input";
            break;
        case SDL_LOG_CATEGORY_TEST:
            cat += "test";
            break;
        case SDL_LOG_CATEGORY_CUSTOM:
            cat += "custom";
            break;
        default:
            cat += "unspecified";
            break;
    }

    switch(priority) {
        case SDL_LOG_PRIORITY_CRITICAL:
        case SDL_LOG_PRIORITY_ERROR:
            log_critical(cat.c_str(), message);
            break;
        case SDL_LOG_PRIORITY_INFO:
            log_info(cat.c_str(), message);
            break;
        case SDL_LOG_PRIORITY_WARN:
            log_warning(cat.c_str(), message);
            break;
        default:
            break;
    }
}

Window create_window(const WindowConfig& configs) {
    WindowBuilder builder(configs.title);
    builder.with_centered_position()
            .with_dimensions(configs.width, configs.height)
            .with_opengl();

    if(configs.allow_resize) {
        builder.as_resizable();
    }

    if(configs.fullscreen == WindowConfig::FullscreenMode::desktop) {
        builder.as_desktop_fullscreen();
    }
    else if(configs.fullscreen == WindowConfig::FullscreenMode::full) {
        builder.as_fullscreen();
    }

    Window window = builder.build();

    if(configs.allow_resize) {
        int min_width = configs.min_width,
            min_height = configs.min_height,
            max_width = configs.max_width,
            max_height = configs.max_height;

        // Setup minimum size
        if(min_width > 0 || min_height > 0) {
            if (min_width == 0) {
                min_width = 640;
            }

            if (min_height == 0) {
                min_height = 480;
            }

            SDL_SetWindowMinimumSize(static_cast<SDL_Window*>(window), min_width, min_height);
        }

        // Setup maximum size
        if(max_width > 0 || max_height > 0) {
            Display window_display = window.current_display();

            if(max_width == 0) {
                max_width = window_display.usable_bounds().width();
            }

            if(max_height == 0) {
                max_height = window_display.usable_bounds().height();
            }

            SDL_SetWindowMaximumSize(static_cast<SDL_Window*>(window), max_width, max_height);
        }
    }

    return window;
}

struct LaunchArgs {
    std::string config_path = "asset/config.xml";

    LaunchArgs() = default;
};

LaunchArgs parse_args(const Arguments& args) {
    LaunchArgs a;

    if(args.size() > 1) {
        a.config_path = args.back();
    }

    return a;
}

#define CASE_KEYBOARD_ALPHA_KEY(sdl_suffix, sushi_key) case SDLK_##sdl_suffix: return Key::sushi_key

constexpr Key sdl_to_sushi_key(int sdl_key) noexcept {
    switch(sdl_key) {
        CASE_KEYBOARD_ALPHA_KEY(a, A);
        CASE_KEYBOARD_ALPHA_KEY(b, B);
        CASE_KEYBOARD_ALPHA_KEY(c, C);
        CASE_KEYBOARD_ALPHA_KEY(d, D);
        CASE_KEYBOARD_ALPHA_KEY(e, E);
        CASE_KEYBOARD_ALPHA_KEY(f, F);
        CASE_KEYBOARD_ALPHA_KEY(g, G);
        CASE_KEYBOARD_ALPHA_KEY(h, H);
        CASE_KEYBOARD_ALPHA_KEY(i, I);
        CASE_KEYBOARD_ALPHA_KEY(j, J);
        CASE_KEYBOARD_ALPHA_KEY(k, K);
        CASE_KEYBOARD_ALPHA_KEY(l, L);
        CASE_KEYBOARD_ALPHA_KEY(m, M);
        CASE_KEYBOARD_ALPHA_KEY(n, N);
        CASE_KEYBOARD_ALPHA_KEY(o, O);
        CASE_KEYBOARD_ALPHA_KEY(p, P);
        CASE_KEYBOARD_ALPHA_KEY(q, Q);
        CASE_KEYBOARD_ALPHA_KEY(r, R);
        CASE_KEYBOARD_ALPHA_KEY(s, S);
        CASE_KEYBOARD_ALPHA_KEY(t, T);
        CASE_KEYBOARD_ALPHA_KEY(u, U);
        CASE_KEYBOARD_ALPHA_KEY(v, V);
        CASE_KEYBOARD_ALPHA_KEY(w, W);
        CASE_KEYBOARD_ALPHA_KEY(x, X);
        CASE_KEYBOARD_ALPHA_KEY(y, Y);
        CASE_KEYBOARD_ALPHA_KEY(z, Z);
        default:
            return Key::Unsupported;
    }
}

#undef CASE_KEYBOARD_ALPHA_KEY

constexpr MouseButton sdl_to_sushi_mouse_button(int sdl_button) noexcept {
    switch(sdl_button) {
        case SDL_BUTTON_LEFT:
            return MouseButton::Left;
        case SDL_BUTTON_RIGHT:
            return MouseButton::Right;
        case SDL_BUTTON_MIDDLE:
            return MouseButton::Middle;
        default:
            return MouseButton::Unsupported;
    }
}

const InputEvent* process_os_event(const SDL_Event& ev, InputFactory& input_factory) {
    switch(ev.type) {
        case SDL_KEYDOWN:
            return input_factory.make_key_event(sdl_to_sushi_key(ev.key.keysym.sym),
                                                KeyState::Pressed);
        case SDL_KEYUP:
            return input_factory.make_key_event(sdl_to_sushi_key(ev.key.keysym.sym),
                                                KeyState::Released);
        case SDL_MOUSEMOTION:
            return input_factory.make_mouse_motion_event(Vec2(ev.motion.x, ev.motion.y),
                                                         Vec2(ev.motion.xrel, ev.motion.yrel));
        case SDL_MOUSEBUTTONDOWN:
            return input_factory.make_mouse_button_event(Vec2(ev.button.x, ev.button.y),
                                                         sdl_to_sushi_mouse_button(ev.button.button),
                                                         MouseButtonState::Down, ev.button.clicks);
        case SDL_MOUSEBUTTONUP:
            return input_factory.make_mouse_button_event(Vec2(ev.button.x, ev.button.y),
                                                         sdl_to_sushi_mouse_button(ev.button.button),
                                                         MouseButtonState::Up, ev.button.clicks);
        default:
            return nullptr;
    }
}

int run_game(BaseGame& game, const Arguments& args, FrameDuration target_frame_duration) {
    sushi::debug::Logger logger;
    sushi::LogService::locate(&logger);
    logger.start();

    LaunchArgs launch = parse_args(args);

    // Load configuration file
    log_info("sushi.bootstrap", "loading configurations...");
    std::ifstream config_stream(launch.config_path);
    XmlDocument config_document(config_stream);
    sushi::GameLoopConfig engine_configurations;
    from_xml(*config_document.root(), engine_configurations);

    sushi::GameLoop loop(engine_configurations);

    log_info("sushi.bootstrap", "creating window");
    sushi::Window main_window = create_window(engine_configurations.window);

    std::unique_ptr<sushi::RendererInterface> renderer = std::make_unique<sushi::OpenGLRenderer>(main_window);
    sushi::StaticMeshBuilderService::locate(&renderer->static_mesh_builder());
    sushi::DebugRendererService::locate(&renderer->debug_renderer());
    sushi::ProxyRenderer proxy_renderer(renderer.get());

    // Input bus setup
    log_info("sushi.bootstrap", "preparing inputs...");
    sushi::InputFactory input_factory;
    sushi::InputBus input_bus;
    sushi::InputBusReader input_bus_reader(input_bus);
    sushi::InputBusService::locate(&input_bus_reader);

    // Input processor setup
    sushi::InputProcessor input_processor;
    sushi::InputProcessorService::locate(&input_processor);

    log_info("sushi.bootstrap", "initializing renderer...");
    if(!renderer->initialize()) {
        return 1;
    }

    log_info("sushi.bootstrap", "starting game loop...");
    game.on_start();

    log_info("sushi.bootstrap", "game loop started");
    // Start of Game loop
    loop.run(target_frame_duration, [&](sushi::FrameDuration last_frame_duration) {
        //==============================================================================================================
        // RENDER CURRENT FRAME
        //==============================================================================================================
        renderer->start_frame_rendering();

        game.on_render(proxy_renderer);

        // Do all rendering here
        renderer->stop_frame_rendering();

        //==============================================================================================================
        // INPUT HANDLING
        //==============================================================================================================
        for(const SDL_Event& ev : sushi::PollEventIterator{}) {
            // Handle quit event first
            if(ev.type == SDL_QUIT) {
                return false;
            }
            else {
                const InputEvent* event = process_os_event(ev, input_factory);

                if(event) {
                    input_bus.push(event);
                }
            }
        }

        input_processor.process();

        //==============================================================================================================
        // UPDATE CURRENT FRAME STATE
        //==============================================================================================================
        // TODO: Pre frame update

        game.on_frame(last_frame_duration);

        // TODO: Frame update

        // Proceed to loop again
        game.on_late_frame(last_frame_duration);

        // TODO: Post frame update

        //==============================================================================================================
        // PREPARATION FOR NEXT FRAME
        //==============================================================================================================
        input_bus.clear();

        return true;
    });

    log_info("sushi.bootstrap", "stopping game loop");
    game.on_stop();

    log_info("sushi.bootstrap", "uninitializing renderer");
    renderer->uninitialize();

    logger.stop();

    return 0;
}

}