#include "opengl_renderer.hpp"
#include "static_mesh_builder.hpp"
#include "opengl.hpp"
#include "log_service.hpp"
#include "string_utils.hpp"
#include "../../sdl/public/window.hpp"

#include <SDL.h>

namespace sushi {

static const char* LOG_CATEGORY = "sushi.renderer.opengl";

class OpenGLStaticMesh : public StaticMesh {
    std::size_t triangles_count;
    gl::vertex_array vao;
    gl::buffer vbo;
public:
    OpenGLStaticMesh() noexcept
    : triangles_count{0}
    , vao{}
    , vbo{} {

    }

    OpenGLStaticMesh(std::size_t size, gl::vertex_array&& vao, gl::buffer&& vbo)
    : triangles_count{size}
    , vao{std::move(vao)}
    , vbo{std::move(vbo)} {
        if(size >= std::numeric_limits<GLsizei>::max() / 3) {
            throw StaticMeshTooLargeError{};
        }
    }

    OpenGLStaticMesh(const OpenGLStaticMesh&) = delete;
    OpenGLStaticMesh& operator=(const OpenGLStaticMesh&) = delete;
    OpenGLStaticMesh(OpenGLStaticMesh&& other) noexcept
    : triangles_count{other.triangles_count}
    , vao{std::move(other.vao)}
    , vbo{std::move(other.vbo)} {
        other.triangles_count = 0;
    }

    OpenGLStaticMesh& operator=(OpenGLStaticMesh&& other) noexcept {
        swap(other);

        return *this;
    }

    void render() override {
        const std::size_t vertex_count = triangles_count * 3;

        gl::bind(vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count));
    }

    void swap(OpenGLStaticMesh& other) noexcept {
        using std::swap;

        vao.swap(other.vao);
        vbo.swap(other.vbo);
        swap(triangles_count, other.triangles_count);
    }
};

class OpenGLStaticMeshBuilder : public StaticMeshBuilder {
public:
    StaticMeshPtr build(const StaticMeshDefinition& definition) override {
        static_assert(sizeof(vertex::Position) == sizeof(FloatPrecision) * 3);

        gl::buffer vbo = gl::buffer::make();
        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(vbo));
        glBufferData(GL_ARRAY_BUFFER,
                     definition.positions().size() * sizeof(vertex::Position),
                     &definition.positions().front().x, GL_STATIC_DRAW);

        gl::vertex_array vao = gl::vertex_array::make();
        gl::bind(vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        gl::bind(gl::vertex_array{});

        return std::make_unique<OpenGLStaticMesh>(definition.positions().size() / 3, std::move(vao), std::move(vbo));
    }
};

struct OpenGLRenderer::impl {
    SDL_GLContext gl_context;
    OpenGLStaticMeshBuilder static_mesh_builder;

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

void OpenGLRenderer::uninitialize() {
    pimpl.reset();
}

}