#ifndef SPOOKYSUSHI_BASE_GAME_HPP
#define SPOOKYSUSHI_BASE_GAME_HPP

#include <chrono>
#include <proxy_renderer.hpp>
#include <editor_endpoint.hpp>

namespace sushi {

using preferred_game_clock = std::chrono::high_resolution_clock;
using frame_duration = preferred_game_clock::duration;

class InputEvent;

class BaseGame {
public:
    // Called once before on_start to setup game specific editor endpoints
#ifdef SUSHI_EDITOR
    virtual void setup_editor(EditorEndpoint& editor) = 0;
#endif

    // Called once when the Game started
    virtual void on_start() = 0;

    /// Called at the start of every frame
    /// \param last_frame The last frame's duration
    virtual void on_frame(frame_duration last_frame) = 0;

    /// Called at the end of every frame
    /// \param last_frame The last frame's duration
    virtual void on_late_frame(frame_duration last_frame) {};

    /// Called every frame to render the current state
    virtual void on_render(sushi::ProxyRenderer renderer) {};

    /// Called once when the Game is stopped
    virtual void on_stop() = 0;

    /// Called when the display surface size changed
    /// \param new_width The new surface's width
    /// \param new_height The new surface's height
    virtual void adapt_to_new_size(uint32_t new_width, uint32_t new_height) {}
};

}

#endif //SPOOKYSUSHI_BASE_GAME_HPP
