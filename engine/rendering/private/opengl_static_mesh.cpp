#include "opengl_static_mesh.hpp"

namespace sushi {

OpenGLStaticMesh::OpenGLStaticMesh(std::size_t element_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer)
: element_count_{element_count}
, vao{std::move(vao)}
, vertices_buffer{std::move(vertices_buffer)}
, normals_buffer{std::move(normals_buffer)}{

}

OpenGLStaticMesh::OpenGLStaticMesh(OpenGLStaticMesh&& other) noexcept
: element_count_{other.element_count_}
, vao{std::move(other.vao)}
, vertices_buffer{std::move(other.vertices_buffer)}
, normals_buffer{std::move(other.normals_buffer)} {
    other.element_count_ = 0;
}

OpenGLStaticMesh& OpenGLStaticMesh::operator=(OpenGLStaticMesh&& other) noexcept {
    swap(other);

    return *this;
}

void OpenGLStaticMesh::swap(OpenGLStaticMesh& other) noexcept {
    using std::swap;

    vao.swap(other.vao);
    vertices_buffer.swap(other.vertices_buffer);
    normals_buffer.swap(other.normals_buffer);
    swap(element_count_, other.element_count_);
}

TrianglesOpenGLStaticMesh::TrianglesOpenGLStaticMesh(std::size_t triangle_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer)
: OpenGLStaticMesh(triangle_count, std::move(vao), std::move(vertices_buffer), std::move(normals_buffer)) {
    if(triangle_count >= std::numeric_limits<GLsizei>::max() / 3) {
        throw StaticMeshTooLargeError{};
    }
}

TrianglesOpenGLStaticMesh::TrianglesOpenGLStaticMesh(TrianglesOpenGLStaticMesh&& other) noexcept
: OpenGLStaticMesh(std::move(other)) {

}

TrianglesOpenGLStaticMesh& TrianglesOpenGLStaticMesh::operator=(TrianglesOpenGLStaticMesh&& other) noexcept {
    swap(other);

    return *this;
}

void TrianglesOpenGLStaticMesh::render() {
    const std::size_t vertex_count = element_count_ * 3;

    gl::bind(vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count));
}

StaticMeshPtr OpenGLStaticMeshBuilder::build(const StaticMeshDefinition& definition) {
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

}