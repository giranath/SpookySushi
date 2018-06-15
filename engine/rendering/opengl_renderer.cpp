#include "opengl_renderer.hpp"
#include "../opengl/opengl.hpp"
#include "../service/log_service.hpp"
#include "../core/string_utils.hpp"
#include "../sdl/window.hpp"

#include <SDL.h>

namespace sushi {

static const char* LOG_CATEGORY = "sushi.renderer.opengl";

struct opengl_renderer::impl {
    SDL_GLContext gl_context;

    ~impl() {
        if(gl_context != 0) {
            SDL_GL_DeleteContext(gl_context);
        }
    }
};

opengl_renderer::opengl_renderer(window& target)
: target_window(target)
, pimpl(std::make_unique<impl>()){

}

opengl_renderer::~opengl_renderer() = default;

bool opengl_renderer::ready() const {
    return pimpl && pimpl->gl_context != 0;
}

bool opengl_renderer::initialize() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    pimpl->gl_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(target_window));
    if(pimpl->gl_context == 0) {
        log_critical(LOG_CATEGORY, SDL_GetError());
        return false;
    }

    gl3wInit();

    return true;
}

void opengl_renderer::start_frame_rendering() {
    // Initializes state for this frame
}

void opengl_renderer::stop_frame_rendering() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do the actual rendering

    SDL_GL_SwapWindow(static_cast<SDL_Window*>(target_window));
}

void opengl_renderer::uninitialize() {
    pimpl.reset();
}

}