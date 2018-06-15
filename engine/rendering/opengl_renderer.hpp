#ifndef SPOOKYSUSHI_OPENGL_RENDERER_HPP
#define SPOOKYSUSHI_OPENGL_RENDERER_HPP

#include "renderer_interface.hpp"
#include "../sdl/window.hpp"

#include <memory>

namespace sushi {
class opengl_renderer : public renderer_interface {
    window& target_window;

    struct impl;
    std::unique_ptr<impl> pimpl;

public:
    explicit opengl_renderer(window& window);
    virtual ~opengl_renderer();

    bool ready() const override;
    void start_frame_rendering() override;
    void stop_frame_rendering() override;
};
}

#endif //SPOOKYSUSHI_OPENGL_RENDERER_HPP
