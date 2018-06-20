#ifndef SPOOKYSUSHI_RENDERER_INTERFACE_HPP
#define SPOOKYSUSHI_RENDERER_INTERFACE_HPP

namespace sushi {

// The client pushes draw commands
// Such as: draw-mesh

class StaticMeshBuilder;

class RendererInterface {
public:
    virtual ~RendererInterface() = default;

    /// Check if the renderer is ready
    virtual bool ready() const = 0;

    virtual bool initialize() = 0;
    virtual void uninitialize() = 0;

    virtual void start_frame_rendering() = 0;
    virtual void stop_frame_rendering() = 0;

    virtual StaticMeshBuilder& static_mesh_builder() const = 0;
};

}

#endif
