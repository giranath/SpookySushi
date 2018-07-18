#ifndef SPOOKYSUSHI_GAME_LOOP_HPP
#define SPOOKYSUSHI_GAME_LOOP_HPP

#include "base_game.hpp"
#include "game_loop_config.hpp"
#include "../async/engine.hpp"
#include "../service/job_service.hpp"
#include "../debug/profiler.hpp"
#include "../debug/logger.hpp"
#include "../service/log_service.hpp"

#include <SDL.h>
#include <stdexcept>
#include <memory>

namespace sushi {

using Argument = std::string;
using Arguments = std::vector<Argument>;
using GameClock = std::chrono::high_resolution_clock;
using FrameDuration = typename GameClock::duration;

int run_game(BaseGame& game, const Arguments& args, FrameDuration target_frame_duration = std::chrono::microseconds(16700));
void sdl_log_output(void* userdata, int category, SDL_LogPriority priority, const char* message);

class GameLoop {
    std::unique_ptr<sushi::async::engine> jobs_;

    void setup_sdl();
    void setup_jobs(const JobConfig& configs);
public:
    using TimePoint = typename GameClock::time_point;

    explicit GameLoop(const GameLoopConfig& config);
    ~GameLoop() noexcept;

    template<typename FN>
    void run(FrameDuration target_frame_duration, FN fn) {
        sushi::debug::Profiler::get().start();
        sushi::JobsService::locate(jobs_.get());
        jobs_->start();

        std::uint64_t frame_index(0);

        bool is_running = true;
        frame_duration last_frame_duration = target_frame_duration;
        while (is_running) {
            sushi::debug::Profiler::get().set_frame_index(frame_index);

            const TimePoint start_of_frame = GameClock::now();

            is_running = fn(last_frame_duration);

            const TimePoint end_of_frame = GameClock::now();
            const FrameDuration frame_duration = end_of_frame - start_of_frame;

            if (frame_duration < target_frame_duration) {
                const FrameDuration missing_duration = target_frame_duration - frame_duration;
                sushi::JobsService::get().foreground()->wait(missing_duration);
                last_frame_duration = GameClock::now() - start_of_frame;
            } else {
                last_frame_duration = frame_duration;
            }

            // Increment to next frame
            ++frame_index;
        }

        jobs_->stop();
        sushi::debug::Profiler::get().stop();
    }


};

}

#endif //SPOOKYSUSHI_GAME_LOOP_HPP
