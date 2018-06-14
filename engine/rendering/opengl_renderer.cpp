#include "opengl_renderer.hpp"
#include "../opengl/opengl.hpp"

#include <SDL.h>

namespace sushi {

opengl_renderer::opengl_renderer(window& target)
: target_window(target) {

}

void opengl_renderer::start_frame_rendering() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void opengl_renderer::stop_frame_rendering() {
	SDL_GL_SwapWindow(static_cast<SDL_Window*>(target_window));
}

}