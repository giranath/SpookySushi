#ifndef SPOOKYSUSHI_RENDERER_INTERFACE_HPP
#define SPOOKYSUSHI_RENDERER_INTERFACE_HPP

namespace sushi {

// The client pushes draw commands
// Such as: draw-mesh

class renderer_interface {
public:
    virtual ~renderer_interface() = default;

    /// Check if the renderer is ready
    virtual bool ready() const = 0;

    virtual bool initialize() = 0;
    virtual void uninitialize() = 0;

    virtual void start_frame_rendering() = 0;
    virtual void stop_frame_rendering() = 0;
};

}

#endif
