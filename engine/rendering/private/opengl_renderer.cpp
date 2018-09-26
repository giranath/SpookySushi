#include "opengl_renderer.hpp"
#include "static_mesh_builder.hpp"
#include "opengl.hpp"
#include "log_service.hpp"
#include "string_utils.hpp"
#include "../../sdl/public/window.hpp"

#include "opengl_static_mesh.hpp"
#include "open_gl_debug_renderer.hpp"

#include <SDL.h>

namespace sushi {

static const char* LOG_CATEGORY = "sushi.renderer.opengl";

struct OpenGLRenderer::impl {
    SDL_GLContext gl_context;
    OpenGLStaticMeshBuilder static_mesh_builder;
    OpenGLDebugRenderer debug_renderer;

    ~impl() {
        if(gl_context != 0) {
            SDL_GL_DeleteContext(gl_context);
        }
    }
};


OpenGLRenderer::OpenGLRenderer(Window& target)
: target_window(target)
, pimpl(std::make_unique<impl>()){

}

OpenGLRenderer::~OpenGLRenderer() noexcept = default;

bool OpenGLRenderer::ready() const {
    return pimpl && pimpl->gl_context != 0;
}

bool OpenGLRenderer::initialize() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    pimpl->gl_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(target_window));
    if(pimpl->gl_context == 0) {
        log_critical(LOG_CATEGORY, SDL_GetError());
        return false;
    }

    gladLoadGL();
    glClearColor(1.f, 1.f, 1.f, 1.f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return true;
}

void OpenGLRenderer::start_frame_rendering() {
    // Initializes state for this frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Move glClear to stop_frame_rendering
}

void OpenGLRenderer::stop_frame_rendering() {
    // Do the actual rendering

    SDL_GL_SwapWindow(static_cast<SDL_Window*>(target_window));
}

StaticMeshBuilder& OpenGLRenderer::static_mesh_builder() const {
    assert(pimpl);
    return pimpl->static_mesh_builder;
}

DebugRendererInterface& OpenGLRenderer::debug_renderer() const {
    assert(pimpl);
    return pimpl->debug_renderer;
}

void OpenGLRenderer::uninitialize() {
    pimpl.reset();
}

}