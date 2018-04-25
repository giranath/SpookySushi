#include "../engine/sdl/window.hpp"
#include "../engine/sdl/event.hpp"
#include "../engine/opengl/debug.hpp"
#include "game_loop.hpp"

#include <toml.hpp>
#include <fstream>
#include <SDL.h>

sushi::window create_window(const toml::Table& window_configs) {
    sushi::window_builder builder(toml::get_or<std::string>(window_configs, "title", "SpookySushi"));
    return builder.with_centered_position()
            .with_dimensions(toml::get_or(window_configs, "width", 800), toml::get_or(window_configs, "height", 600))
            .with_opengl()
            .as_resizable()
            .build();
}

int main() {
    std::ifstream config_stream("asset/config.toml");
    auto configs = toml::parse(config_stream);

    game_loop loop;
    sushi::window main_window = create_window(toml::get<toml::Table>(configs.at("window")));

    // Load Gl3w functions
    gl3wInit();

    // List opengl extensions
    for(const std::string& extension : sushi::gl::extension_iterator{}) {
        std::cout << extension << std::endl;
    }

    // Start of game loop
    loop.run([&main_window]() {
        // Handle inputs
        for(const SDL_Event& ev : sushi::poll_event_iterator{}) {
            if(ev.type == SDL_QUIT) {
                // Loop is interrupted
                return false;
            }
        }

        // Update game state

        // Render current frame

        // Proceed to loop again
        return true;
    });
}