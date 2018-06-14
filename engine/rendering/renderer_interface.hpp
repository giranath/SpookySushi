#ifndef SPOOKYSUSHI_RENDERER_INTERFACE_HPP
#define SPOOKYSUSHI_RENDERER_INTERFACE_HPP

namespace sushi {

class renderer_interface {
public:
	virtual void start_frame_rendering() = 0;
	virtual void stop_frame_rendering() = 0;
};

}

#endif
