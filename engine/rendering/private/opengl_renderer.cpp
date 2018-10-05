#include "opengl_renderer.hpp"
#include "static_mesh_builder.hpp"
#include "opengl.hpp"
#include "log_service.hpp"
#include "string_utils.hpp"
#include <window.hpp>

#include <SDL.h>

namespace sushi {

static const char* LOG_CATEGORY = "sushi.renderer.opengl";

class OpenGLStaticMesh : public StaticMesh {
protected:
    std::size_t element_count_ = {};
    gl::vertex_array vao;
    gl::buffer vertices_buffer;
    gl::buffer normals_buffer;
public:
    OpenGLStaticMesh() noexcept = default;

    OpenGLStaticMesh(std::size_t element_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer)
    : element_count_{element_count}
    , vao{std::move(vao)}
    , vertices_buffer{std::move(vertices_buffer)}
    , normals_buffer{std::move(normals_buffer)}{

    }

    OpenGLStaticMesh(const OpenGLStaticMesh&) = delete;
    OpenGLStaticMesh& operator=(const OpenGLStaticMesh&) = delete;

    OpenGLStaticMesh(OpenGLStaticMesh&& other) noexcept
    : element_count_{other.element_count_}
    , vao{std::move(other.vao)}
    , vertices_buffer{std::move(other.vertices_buffer)}
    , normals_buffer{std::move(other.normals_buffer)} {
        other.element_count_ = 0;
    }

    OpenGLStaticMesh& operator=(OpenGLStaticMesh&& other) noexcept {
        swap(other);

        return *this;
    }

    void swap(OpenGLStaticMesh& other) noexcept {
        using std::swap;

        vao.swap(other.vao);
        vertices_buffer.swap(other.vertices_buffer);
        normals_buffer.swap(other.normals_buffer);
        swap(element_count_, other.element_count_);
    }
};

class TrianglesOpenGLStaticMesh : public OpenGLStaticMesh {
public:
    TrianglesOpenGLStaticMesh() noexcept = default;

    TrianglesOpenGLStaticMesh(std::size_t triangle_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer)
    : OpenGLStaticMesh(triangle_count, std::move(vao), std::move(vertices_buffer), std::move(normals_buffer)) {
        if(triangle_count >= std::numeric_limits<GLsizei>::max() / 3) {
            throw StaticMeshTooLargeError{};
        }
    }

    TrianglesOpenGLStaticMesh(TrianglesOpenGLStaticMesh&& other) noexcept
    : OpenGLStaticMesh(std::move(other)) {

    }

    TrianglesOpenGLStaticMesh& operator=(TrianglesOpenGLStaticMesh&& other) noexcept {
        swap(other);

        return *this;
    }

    void render() override {
        const std::size_t vertex_count = element_count_ * 3;

        gl::bind(vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count));
    }
};

template<typename IndexType>
struct is_buffer_index {
    using value_type = IndexType;
    enum { value = false };
};

template<>
struct is_buffer_index<uint8_t> {
    using value_type = uint8_t;
    enum { value = true };
};

template<>
struct is_buffer_index<uint16_t> {
    using value_type = uint16_t;
    enum { value = true };
};

template<>
struct is_buffer_index<uint32_t> {
    using value_type = uint32_t;
    enum { value = true };
};

template<>
struct is_buffer_index<uint64_t> {
    using value_type = uint64_t;
    enum { value = true };
};

template<typename IndexType = uint16_t>
class IndexedOpenGLStaticMesh : public OpenGLStaticMesh {
    gl::buffer element_buffer;
public:
    static_assert(is_buffer_index<IndexType>::value, "invalid index type");
    IndexedOpenGLStaticMesh() noexcept = default;
    IndexedOpenGLStaticMesh(std::size_t element_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer, gl::buffer&& indices_buffer)
    : OpenGLStaticMesh(element_count, std::move(vao), std::move(vertices_buffer), std::move(normals_buffer))
    , element_buffer(std::move(indices_buffer)) {
        if(element_count >= std::numeric_limits<GLsizei>::max()) {
            throw StaticMeshTooLargeError{};
        }
    }

    IndexedOpenGLStaticMesh(IndexedOpenGLStaticMesh&& other) noexcept
    : OpenGLStaticMesh(std::move(other))
    , element_buffer(std::move(other.element_buffer)) {

    }

    IndexedOpenGLStaticMesh& operator=(IndexedOpenGLStaticMesh&& other) noexcept {
        swap(other);

        return *this;
    }

    void swap(IndexedOpenGLStaticMesh& other) noexcept {
        using std::swap;

        using std::swap;
        OpenGLStaticMesh::swap(other);
        swap(element_buffer, other.element_buffer);
    }

    void render() override {
        gl::bind(vao);
        gl::bind(gl::buffer_bind<GL_ELEMENT_ARRAY_BUFFER>(element_buffer));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(element_count_), GL_UNSIGNED_SHORT, nullptr);
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
                     definition.positions().data(), GL_STATIC_DRAW);

        gl::buffer normals = gl::buffer::make();
        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(normals));
        glBufferData(GL_ARRAY_BUFFER,
                     definition.normals().size() * sizeof(vertex::Normal),
                     definition.normals().data(), GL_STATIC_DRAW);

        gl::buffer elements = gl::buffer::make();
        gl::bind(gl::buffer_bind<GL_ELEMENT_ARRAY_BUFFER>(elements));
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     definition.indices().size() * sizeof(vertex::Indice),
                     &definition.indices().front(), GL_STATIC_DRAW);

        gl::vertex_array vao = gl::vertex_array::make();
        gl::bind(vao);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(vbo));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(normals));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);

        gl::bind(gl::vertex_array{});

        if(definition.uses_indices()) {
            return std::make_unique<IndexedOpenGLStaticMesh<>>(definition.indices().size(), std::move(vao), std::move(vbo), std::move(normals), std::move(elements));
        }
        else {
            return std::make_unique<TrianglesOpenGLStaticMesh>(definition.positions().size() / 3, std::move(vao), std::move(vbo), std::move(normals));
        }
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

    if (!gladLoadGL()) {
        log_critical("sushi.renderer.opengl", "failed to load OpenGL functions");
        return false;
    }

    if (SDL_GL_MakeCurrent(static_cast<SDL_Window*>(target_window), pimpl->gl_context) != 0) {
        log_critical("sushi.renderer.opengl", "failed to make OpenGL context current : %s", SDL_GetError());
        return false;
    }

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

void OpenGLRenderer::uninitialize() {
    pimpl.reset();
}

}