#include "game_loop.hpp"
#include "../sdl/window.hpp"
#include "../sdl/event.hpp"
#include "../opengl/opengl.hpp"

#include <toml.hpp>
#include <SDL.h>

namespace sushi {

struct window_configs {
    std::string title;
    int width;
    int height;
    int min_width;
    int min_height;
};

void from_toml(const toml::Table& table, window_configs& configs) {
    configs.width = toml::get_or(table, "width", 800);
    configs.height = toml::get_or(table, "height", 600);
    configs.title = toml::get_or<std::string>(table, "title", "Game");
    configs.min_width = toml::get_or(table, "min_width", 640);
    configs.min_height = toml::get_or(table, "min_height", 480);
}

sushi::window create_window(const window_configs& configs) {
    sushi::window_builder builder(configs.title);
    sushi::window window = builder.with_centered_position()
            .with_dimensions(configs.width, configs.height)
            .with_opengl()
            .as_resizable()
            .build();


    SDL_SetWindowMinimumSize(static_cast<SDL_Window*>(window), configs.min_width, configs.min_height);
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
    launch_args launch = parse_args(args);

    std::ifstream config_stream(launch.config_path);
    auto configs = toml::parse(config_stream);

    sushi::game_loop loop(configs);
    window_configs window_confs;
    from_toml(toml::get<toml::Table>(configs.at("window")), window_confs);
    sushi::window main_window = create_window(window_confs);

    // Load Gl3w functions
    gl3wInit();

    // List opengl extensions
    for(const std::string& extension : sushi::gl::extension_iterator{}) {
        std::cout << extension << std::endl;
    }

    game.on_start();

    // Start of game loop
    loop.run(std::chrono::milliseconds(16), [&game](sushi::game_loop::duration last_frame_duration) {
        // Handle inputs
        for(const SDL_Event& ev : sushi::poll_event_iterator{}) {
            if(ev.type == SDL_QUIT) {
                // Loop is interrupted
                return false;
            }
        }

        // Update game state
        game.on_frame(last_frame_duration);

        // Render current frame

        // Proceed to loop again
        game.on_late_frame(last_frame_duration);

        return true;
    });

    game.on_stop();
}

}