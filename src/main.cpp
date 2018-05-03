#include "../engine/sdl/window.hpp"
#include "../engine/sdl/event.hpp"
#include "../engine/opengl/debug.hpp"
#include "../engine/async/job.hpp"
#include "game_loop.hpp"
#include "../engine/async/worker.hpp"
#include "../engine/async/engine.hpp"
#include "../engine/debug/profiler.hpp"
#include "../engine/ecs/tree_storage.hpp"
#include "../engine/ecs/indirect_vector_storage.hpp"
#include "../engine/ecs/entity_iterator.hpp"

#include <toml.hpp>
#include <fstream>
#include <SDL.h>

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

const uint32_t JOB_PROFILE_NAME = 315135;
const uint32_t FRAME_PROFILE_NAME = 666;

int main() {
    sushi::ecs::indirect_vector_storage<int> integers_storage;
    sushi::ecs::indirect_vector_storage<double> doubles_storage;
    sushi::ecs::indirect_vector_storage<float> floats_storage;

    integers_storage.emplace(12, 3);
    doubles_storage.emplace(12, 3.14159);
    floats_storage.emplace(12, 3.14159f);

    integers_storage.emplace(2, 351);
    doubles_storage.emplace(2, 14.2);
    floats_storage.emplace(2, 3.14159f);

    for(auto p : sushi::ecs::entity_iterator(integers_storage, doubles_storage, floats_storage)) {
        int a;
        double b;
        float c;

        std::tie(a, b, c) = p.second;
        std::cout << p.first << " " << a << ", " << b << ", " << c << std::endl;
        std::get<1>(p.second) = 0.0;
    }

    for(auto p : sushi::ecs::entity_iterator(integers_storage, doubles_storage, floats_storage)) {
        int a;
        double b;
        float c;

        std::tie(a, b, c) = p.second;
        std::cout << p.first << " " << a << ", " << b << ", " << c << std::endl;
    }

    std::ifstream config_stream("asset/config.toml");
    auto configs = toml::parse(config_stream);

    game_loop loop(configs);
    window_configs window_confs;
    from_toml(toml::get<toml::Table>(configs.at("window")), window_confs);
    sushi::window main_window = create_window(window_confs);

    // Load Gl3w functions
    gl3wInit();

    // List opengl extensions
    for(const std::string& extension : sushi::gl::extension_iterator{}) {
        std::cout << extension << std::endl;
    }

    std::once_flag once;

    // Start of game loop
    loop.run(std::chrono::milliseconds(16), [&main_window, &once](game_loop::duration last_frame_duration) {
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(last_frame_duration).count() << std::endl;
        sushi::debug::scoped_profile profile(FRAME_PROFILE_NAME);
        std::call_once(once, []() {
            for(int i = 0; i < 10000; ++i) {
                sushi::jobs_service::get().make_job(sushi::async::worker::mode::background, [](sushi::async::job &job) {
                    sushi::debug::scoped_profile profile(JOB_PROFILE_NAME);
                    std::this_thread::sleep_for(std::chrono::milliseconds(3));
                }, i);
            }
        });

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