#ifndef SPOOKYSUSHI_OPENGL_RENDERER_HPP
#define SPOOKYSUSHI_OPENGL_RENDERER_HPP

#include "renderer_interface.hpp"

#include <memory>

namespace sushi {

class Window;

class OpenGLRenderer : public RendererInterface {
    Window& target_window;

    struct impl;
    std::unique_ptr<impl> pimpl;

public:
    explicit OpenGLRenderer(Window& window);
    virtual ~OpenGLRenderer() noexcept;

    bool ready() const override;

    bool initialize() override;
    void uninitialize() override;

    void start_frame_rendering() override;
    void stop_frame_rendering() override;

    StaticMeshBuilder& static_mesh_builder() const override;
    DebugRendererInterface& debug_renderer() const override;
};
}

#endif //SPOOKYSUSHI_OPENGL_RENDERER_HPP
