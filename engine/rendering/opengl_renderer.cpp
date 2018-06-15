#include "opengl_renderer.hpp"
#include "../opengl/opengl.hpp"
#include "../service/log_service.hpp"
#include "../core/string_utils.hpp"

#include <SDL.h>

namespace sushi {

struct opengl_renderer::impl {
    SDL_GLContext gl_context;
};

opengl_renderer::opengl_renderer(window& target)
: target_window(target)
, pimpl(std::make_unique<impl>()){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    pimpl->gl_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(target_window));
    if(pimpl->gl_context != 0) {
        gl3wInit();
    }
    else {
        log_critical("renderer", "%s", SDL_GetError());
    }
}

opengl_renderer::~opengl_renderer() = default;

bool opengl_renderer::ready() const {
    return pimpl && pimpl->gl_context != 0;
}

void opengl_renderer::start_frame_rendering() {
    // Initializes state for this frame
}

void opengl_renderer::stop_frame_rendering() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Do the actual rendering

    SDL_GL_SwapWindow(static_cast<SDL_Window*>(target_window));
}

}