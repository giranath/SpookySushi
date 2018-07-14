#include "game_loop.hpp"
#include "../sdl/window.hpp"
#include "../sdl/event.hpp"
#include "../sdl/display.hpp"
#include "../opengl/opengl.hpp"
#include "../rendering/renderer_interface.hpp"
#include "../rendering/opengl_renderer.hpp"
#include "../rendering/proxy_renderer.hpp"
#include "../service/static_mesh_builder_service.hpp"
#include "../service/input_bus_service.hpp"
#include "../input/input_factory.hpp"
#include "../input/input_event.hpp"
#include "../input/input_bus.hpp"
#include "../service/service_locator.hpp"

#include <toml.hpp>
#include <SDL.h>

namespace sushi {

GameLoop::GameLoop(const toml::Table &configs)
: jobs_(nullptr) {
    setup_sdl(configs);
    setup_jobs(configs);
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

struct WindowConfig {
    enum class FullscreenMode {
        none,
        desktop,
        full
    };

    std::string title;
    int width;
    int height;
    int min_width;
    int min_height;
    int max_width;
    int max_height;
    bool allow_resize;
    FullscreenMode fullscreen;
};

void from_toml(const toml::Table& table, WindowConfig& configs) {
    configs.width = toml::get_or(table, "width", 800);
    configs.height = toml::get_or(table, "height", 600);
    configs.title = toml::get_or<std::string>(table, "title", "Game");
    configs.min_width = toml::get_or(table, "min_width", 0); // 640
    configs.min_height = toml::get_or(table, "min_height", 0); // 480
    configs.max_width = toml::get_or(table, "max_width", 0);
    configs.max_height = toml::get_or(table, "max_height", 0);
    configs.allow_resize = toml::get_or(table, "allow_resize", false);

    std::string fullscreen = toml::get_or<std::string>(table, "fullscreen", "none");
    std::transform(std::begin(fullscreen), std::end(fullscreen), std::begin(fullscreen), [](char l) { return std::tolower(l); });

    if(fullscreen == "none") {
        configs.fullscreen = WindowConfig::FullscreenMode::none;
    }
    else if(fullscreen == "desktop") {
        configs.fullscreen = WindowConfig::FullscreenMode::desktop;
    }
    else if(fullscreen == "full") {
        configs.fullscreen = WindowConfig::FullscreenMode::full;
    }
    else {
        configs.fullscreen = WindowConfig::FullscreenMode::none;

        log_warning("sushi.config.window", "invalid fullscreen mode in config.toml");
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
    std::string config_path = "asset/config.toml";

    LaunchArgs() = default;
};

LaunchArgs parse_args(const Arguments& args) {
    LaunchArgs a;

    if(args.size() > 1) {
        a.config_path = args.back();
    }

    return a;
}

constexpr Key sdl_to_sushi_key(int sdl_key) noexcept {
    switch(sdl_key) {
        case SDLK_a:
            return Key::A;
        case SDLK_w:
            return Key::W;
        case SDLK_d:
            return Key::D;
        case SDLK_s:
            return Key::S;
        default:
            return Key::Unsupported;
    }
}

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

    std::ifstream config_stream(launch.config_path);
    auto configs = toml::parse(config_stream);

    sushi::GameLoop loop(configs);
    WindowConfig window_confs;
    from_toml(toml::get<toml::Table>(configs.at("window")), window_confs);
    sushi::Window main_window = create_window(window_confs);

    std::unique_ptr<sushi::RendererInterface> renderer = std::make_unique<sushi::OpenGLRenderer>(main_window);
    sushi::StaticMeshBuilderService::locate(&renderer->static_mesh_builder());
    sushi::ProxyRenderer proxy_renderer(renderer.get());

    sushi::InputFactory input_factory;

    sushi::InputBus input_bus;
    sushi::InputBusReader input_bus_reader(input_bus);
    sushi::InputBusService::locate(&input_bus_reader);

    if(!renderer->initialize()) {
        return 1;
    }

    game.on_start();

    // Start of Game loop
    loop.run(target_frame_duration, [&](sushi::FrameDuration last_frame_duration) {
        //==============================================================================================================
        // INPUT HANDLING
        //==============================================================================================================
        for(const SDL_Event& ev : sushi::PollEventIterator{}) {
            // Handle quit event first
            if(ev.type == SDL_QUIT) {
                return false;
            }
            else {
                input_bus.push(process_os_event(ev, input_factory));
            }
        }

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
        // RENDER CURRENT FRAME
        //==============================================================================================================
        renderer->start_frame_rendering();

        game.on_render(proxy_renderer);

        // Do all rendering here
        renderer->stop_frame_rendering();

        return true;
    });

    game.on_stop();

    renderer->uninitialize();

    logger.stop();

    return 0;
}

}