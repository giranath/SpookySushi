#include "vertex_array.hpp"

#include <algorithm>

namespace gl {

vertex_array::vertex_array(GLuint vao) noexcept
: vao{vao} {

}

vertex_array::vertex_array(vertex_array&& other) noexcept
: vao{other.vao} {

}

vertex_array& vertex_array::operator=(vertex_array&& other) noexcept {
    std::swap(vao, other.vao);

    return *this;
}

vertex_array::~vertex_array() noexcept {
    glDeleteVertexArrays(1, &vao);
}

bool vertex_array::good() const noexcept {
    return vao != 0;
}

vertex_array::operator GLuint() const noexcept {
    return vao;
}

void vertex_array::bind() const noexcept {
    glBindVertexArray(vao);
}

void vertex_array::swap(vertex_array& other) noexcept {
    std::swap(vao, other.vao);
}

vertex_array vertex_array::make() noexcept {
    GLuint vao;
    glGenVertexArrays(1, &vao);

    return vertex_array(vao);
}


}