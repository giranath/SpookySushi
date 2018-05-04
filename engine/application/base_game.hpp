#ifndef SPOOKYSUSHI_BASE_GAME_HPP
#define SPOOKYSUSHI_BASE_GAME_HPP

#include <chrono>

namespace sushi {

using preferred_game_clock = std::chrono::high_resolution_clock;
using frame_duration = preferred_game_clock::duration;

class base_game {
public:
    // Called once when the game started
    virtual void on_start() = 0;

    /// Called at the start of every frame
    /// \param last_frame The last frame's duration
    virtual void on_frame(frame_duration last_frame) = 0;

    /// Called at the end of every frame
    /// \param last_frame The last frame's duration
    virtual void on_late_frame(frame_duration last_frame) {};

    // Called once when the game is stopped
    virtual void on_stop() = 0;
};

}

#endif //SPOOKYSUSHI_BASE_GAME_HPP
