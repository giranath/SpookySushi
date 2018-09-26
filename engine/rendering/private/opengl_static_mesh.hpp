#ifndef SPOOKYSUSHI_OPEN_GL_STATIC_MESH_HPP
#define SPOOKYSUSHI_OPEN_GL_STATIC_MESH_HPP

#include <opengl.hpp>
#include "static_mesh_builder.hpp"

namespace sushi {

class OpenGLStaticMesh : public StaticMesh {
protected:
    std::size_t element_count_ = {};
    gl::vertex_array vao;
    gl::buffer vertices_buffer;
    gl::buffer normals_buffer;
public:
    OpenGLStaticMesh() noexcept = default;

    OpenGLStaticMesh(std::size_t element_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer);

    OpenGLStaticMesh(const OpenGLStaticMesh&) = delete;
    OpenGLStaticMesh& operator=(const OpenGLStaticMesh&) = delete;

    OpenGLStaticMesh(OpenGLStaticMesh&& other) noexcept;

    OpenGLStaticMesh& operator=(OpenGLStaticMesh&& other) noexcept;

    void swap(OpenGLStaticMesh& other) noexcept;
};

class TrianglesOpenGLStaticMesh : public OpenGLStaticMesh {
public:
    TrianglesOpenGLStaticMesh() noexcept = default;

    TrianglesOpenGLStaticMesh(std::size_t triangle_count, gl::vertex_array&& vao, gl::buffer&& vertices_buffer, gl::buffer&& normals_buffer);
    TrianglesOpenGLStaticMesh(TrianglesOpenGLStaticMesh&& other) noexcept;
    TrianglesOpenGLStaticMesh& operator=(TrianglesOpenGLStaticMesh&& other) noexcept;
    void render() override;
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
    StaticMeshPtr build(const StaticMeshDefinition& definition) override;
};

}

#endif
