#include "../engine/sdl/window.hpp"
#include "../engine/sdl/event.hpp"
#include "../engine/opengl/debug.hpp"
#include "../engine/async/job.hpp"
#include "game_loop.hpp"
#include "../engine/async/worker.hpp"
#include "../engine/async/engine.hpp"

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

std::mutex cout_mutex;

int main() {
    std::ifstream config_stream("asset/config.toml");
    auto configs = toml::parse(config_stream);

    game_loop loop;
    sushi::window main_window = create_window(toml::get<toml::Table>(configs.at("window")));

    SDL_SetWindowMinimumSize(static_cast<SDL_Window*>(main_window), 800, 600);

    // Load Gl3w functions
    gl3wInit();

    // List opengl extensions
    for(const std::string& extension : sushi::gl::extension_iterator{}) {
        std::cout << extension << std::endl;
    }

    sushi::async::engine engine(std::thread::hardware_concurrency() - 1, 2048);
    engine.start();

    auto start = std::chrono::high_resolution_clock::now();
    sushi::async::job root_job;
    for(int i = 0; i < 100; ++i) {
        engine.make_job(sushi::async::worker::mode::background, [](sushi::async::job& job) {
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "#" << job.data<int>() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(job.data<int>() % 3));
        }, i, &root_job);
    }

    engine.foreground()->wait_for(&root_job);
    auto duration = std::chrono::high_resolution_clock::now() - start;
    std::cout << "took " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms" << std::endl;

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

    engine.stop();
}