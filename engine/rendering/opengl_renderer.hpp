#ifndef SPOOKYSUSHI_OPENGL_RENDERER_HPP
#define SPOOKYSUSHI_OPENGL_RENDERER_HPP

#include "renderer_interface.hpp"
#include "../sdl/window.hpp"

namespace sushi {
class opengl_renderer : public renderer_interface {
	window& target_window;
public:
	explicit opengl_renderer(window& window);
	void start_frame_rendering() override;
	void stop_frame_rendering() override;
};
}

#endif //SPOOKYSUSHI_OPENGL_RENDERER_HPP
