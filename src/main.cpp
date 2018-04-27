#include "../engine/sdl/window.hpp"
#include "../engine/sdl/event.hpp"
#include "../engine/opengl/debug.hpp"
#include "../engine/async/job.hpp"
#include "game_loop.hpp"
#include "../engine/async/worker.hpp"
#include "../engine/async/engine.hpp"
#include "../engine/debug/profiler.hpp"

#include <toml.hpp>
#include <fstream>
#include <SDL.h>

sushi::window create_window(const toml::Table& window_configs) {
    sushi::window_builder builder(toml::get_or<std::string>(window_configs, "title", "Game"));
    sushi::window window = builder.with_centered_position()
                          .with_dimensions(toml::get_or(window_configs, "width", 800), toml::get_or(window_configs, "height", 600))
                          .with_opengl()
                          .as_resizable()
                          .build();


    SDL_SetWindowMinimumSize(static_cast<SDL_Window*>(window),
                             toml::get_or(window_configs, "min_width", 640),
                             toml::get_or(window_configs, "min_height", 480));
    return window;
}

const uint32_t JOB_PROFILE_NAME = 315135;

int main() {
    std::ifstream config_stream("asset/config.toml");
    auto configs = toml::parse(config_stream);

    game_loop loop(configs);
    sushi::window main_window = create_window(toml::get<toml::Table>(configs.at("window")));

    // Load Gl3w functions
    gl3wInit();

    // List opengl extensions
    for(const std::string& extension : sushi::gl::extension_iterator{}) {
        std::cout << extension << std::endl;
    }

    std::once_flag once;

    sushi::debug::profiler::get().start();
    sushi::async::job root;
    // Start of game loop


    loop.run([&main_window, &once, &root]() {
        std::call_once(once, [&root]() {
            for(int i = 0; i < 10000; ++i) {
                sushi::jobs_service::get().make_job(sushi::async::worker::mode::background, [](sushi::async::job &job) {
                    sushi::debug::profiler::get().push(sushi::debug::profile_event{JOB_PROFILE_NAME, sushi::debug::profile_event::type::start});

                    std::this_thread::sleep_for(std::chrono::milliseconds(3));

                    sushi::debug::profiler::get().push(sushi::debug::profile_event{JOB_PROFILE_NAME, sushi::debug::profile_event::type::end});
                }, i, &root);
            }
        });

        if(sushi::jobs_service::get().foreground()->wait_for(&root, std::chrono::milliseconds(1))) {
            return false;
        }

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

    sushi::debug::profiler::get().stop();
}