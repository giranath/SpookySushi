#include "../engine/sdl/window.hpp"
#include "../engine/sdl/event.hpp"
#include "../engine/opengl/debug.hpp"
#include "../engine/async/job.hpp"
#include "game_loop.hpp"
#include "../engine/async/worker.hpp"

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

    SDL_SetWindowMinimumSize(static_cast<SDL_Window*>(main_window), 800, 600);

    // Load Gl3w functions
    gl3wInit();

    // List opengl extensions
    for(const std::string& extension : sushi::gl::extension_iterator{}) {
        std::cout << extension << std::endl;
    }

    struct huge_data {
        int d[1024];
    };

    huge_data huge;
    static_assert(std::is_pod<huge_data>::value);

    /*
    sushi::async::job j([](sushi::async::job& job) {
        auto message = job.data<std::string>();

        std::cout << message << std::endl;
    });
    std::string message = "Yo bitch!";
    sushi::async::job other;
    sushi::async::job::make_closure(&other, &j, [&message, huge](sushi::async::job& job) {
        std::cout << message << std::endl;
    });
    j.emplace<std::string>("Hello world!");
    j.execute();

    assert(!j.is_finished());

    other.execute();
    assert(other.is_finished());
    assert(j.is_finished());
     */
    sushi::async::job j([](sushi::async::job& job) {
        std::cout << "job done!" << std::endl;
    });
    sushi::async::worker worker;
    worker.start();

    std::cout << "waiting..." << std::endl;
    worker.wait_for(&j, std::chrono::seconds(10));
    std::cout << "waited" << std::endl;

    worker.stop();

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