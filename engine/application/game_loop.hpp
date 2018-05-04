#ifndef SPOOKYSUSHI_GAME_LOOP_HPP
#define SPOOKYSUSHI_GAME_LOOP_HPP

#include "../async/engine.hpp"
#include "../service/job_service.hpp"
#include "../debug/profiler.hpp"
#include "base_game.hpp"

#include <toml.hpp>
#include <SDL.h>
#include <stdexcept>
#include <memory>

namespace sushi {

using argument = std::string;
using arguments = std::vector<argument>;

int run_game(base_game& game, const arguments& args);

class game_loop {
    std::unique_ptr<sushi::async::engine> jobs_;

    void setup_sdl(const toml::Table &configs) {
        // Setup SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw std::runtime_error("cannot initialize SDL");
        }
    }

    void setup_jobs(const toml::Table &configs) {
        // Setup the job manager
        std::size_t worker_count = std::thread::hardware_concurrency() - 1;
        std::size_t max_job_count = 2048;

        auto it = configs.find("jobs");
        if (it != configs.end()) {
            const toml::Table &jobs_configs = toml::get<toml::Table>(it->second);

            worker_count = toml::get_or(jobs_configs, "worker_count", worker_count);
            max_job_count = toml::get_or(jobs_configs, "max_job_count", max_job_count);
        }

        jobs_ = std::make_unique<sushi::async::engine>(worker_count, max_job_count);
    }

public:
    using clock = std::chrono::high_resolution_clock;
    using duration = clock::duration;
    using time_point = clock::time_point;

    explicit game_loop(const toml::Table &configs)
            : jobs_(nullptr) {
        setup_sdl(configs);
        setup_jobs(configs);
    }

    template<typename FN>
    void run(duration target_frame_duration, FN fn) {
        sushi::debug::profiler::get().start();
        sushi::jobs_service::locate(jobs_.get());
        jobs_->start();

        // TODO: Setup profiler
        std::atomic_uint64_t frame_index(0);

        bool is_running = true;
        duration last_frame_duration = target_frame_duration;
        while (is_running) {
            const time_point start_of_frame = clock::now();

            is_running = fn(last_frame_duration);

            const time_point end_of_frame = clock::now();
            const duration frame_duration = end_of_frame - start_of_frame;

            if (frame_duration < target_frame_duration) {
                const duration missing_duration = target_frame_duration - frame_duration;
                sushi::jobs_service::get().foreground()->wait(missing_duration);
                last_frame_duration = clock::now() - start_of_frame;
            } else {
                last_frame_duration = frame_duration;
            }

            // Increment to next frame
            ++frame_index;
        }

        jobs_->stop();
        sushi::debug::profiler::get().stop();
    }

    ~game_loop() {
        SDL_Quit();
    }
};

}

#endif //SPOOKYSUSHI_GAME_LOOP_HPP
