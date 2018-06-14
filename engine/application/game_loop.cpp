#include "game_loop.hpp"
#include "../sdl/window.hpp"
#include "../sdl/event.hpp"
#include "../sdl/display.hpp"
#include "../opengl/opengl.hpp"
#include "../rendering/renderer_interface.hpp"
#include "../rendering/opengl_renderer.hpp"

#include <toml.hpp>
#include <SDL.h>

namespace sushi {

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
            log_service::get().log_critical(cat, message);
            break;
        case SDL_LOG_PRIORITY_INFO:
            log_service::get().log_info(cat, message);
            break;
        case SDL_LOG_PRIORITY_WARN:
            log_service::get().log_warning(cat, message);
            break;
        default:
            break;
    }
}

struct window_configs {
    enum class fullscreen_mode {
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
    fullscreen_mode fullscreen;
};

void from_toml(const toml::Table& table, window_configs& configs) {
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
        configs.fullscreen = window_configs::fullscreen_mode::none;
    }
    else if(fullscreen == "desktop") {
        configs.fullscreen = window_configs::fullscreen_mode::desktop;
    }
    else if(fullscreen == "full") {
        configs.fullscreen = window_configs::fullscreen_mode::full;
    }
    else {
        configs.fullscreen = window_configs::fullscreen_mode::none;
        // TODO: Change category
        log_service::get().log_warning("sushi.config.window", "invalid fullscreen mode in config.toml");
    }
}

sushi::window create_window(const window_configs& configs) {
    sushi::window_builder builder(configs.title);
    builder.with_centered_position()
            .with_dimensions(configs.width, configs.height)
            .with_opengl();

    if(configs.allow_resize) {
        builder.as_resizable();
    }

    if(configs.fullscreen == window_configs::fullscreen_mode::desktop) {
        builder.as_desktop_fullscreen();
    }
    else if(configs.fullscreen == window_configs::fullscreen_mode::full) {
        builder.as_fullscreen();
    }

    sushi::window window = builder.build();

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
            display window_display = window.current_display();

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

struct launch_args {
    std::string config_path = "asset/config.toml";

    launch_args() = default;
};

// sushi <config_path>
launch_args parse_args(const arguments& args) {
    launch_args a;

    if(args.size() > 1) {
        a.config_path = args.back();
    }

    return a;
}

int run_game(base_game& game, const arguments& args) {
    sushi::debug::logger logger;
    sushi::log_service::locate(&logger);
    logger.start();

    launch_args launch = parse_args(args);

    std::ifstream config_stream(launch.config_path);
    auto configs = toml::parse(config_stream);

    sushi::game_loop loop(configs);
    window_configs window_confs;
    from_toml(toml::get<toml::Table>(configs.at("window")), window_confs);
    sushi::window main_window = create_window(window_confs);

    std::unique_ptr<sushi::renderer_interface> renderer = std::make_unique<sushi::opengl_renderer>(main_window);

    // Load Gl3w functions
    gl3wInit();

    game.on_start();

    // Start of game loop
    loop.run(std::chrono::milliseconds(16), [&](sushi::game_loop::duration last_frame_duration) {
        // Handle inputs
        for(const SDL_Event& ev : sushi::poll_event_iterator{}) {
            if(ev.type == SDL_QUIT) {
                // Loop is interrupted
                return false;
            }
        }

        // Update game state
        game.on_frame(last_frame_duration);

        // Proceed to loop again
        game.on_late_frame(last_frame_duration);

        // Render current frame
        renderer->start_frame_rendering();
        renderer->stop_frame_rendering();

        return true;
    });

    game.on_stop();

    logger.stop();

    return 0;
}

}