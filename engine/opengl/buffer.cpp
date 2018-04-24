#include "buffer.hpp"

#include <algorithm>

namespace gl {

buffer::buffer(GLuint vbo) noexcept
: vbo{vbo} {

}

buffer::buffer(buffer&& other) noexcept
: vbo{other.vbo} {
    other.vbo = 0;
}

buffer& buffer::operator=(buffer&& other) noexcept {
    std::swap(vbo, other.vbo);
    return *this;
}

buffer::~buffer() noexcept {
    glDeleteBuffers(1, &vbo);
}

bool buffer::good() const noexcept {
    return vbo != 0;
}

buffer::operator GLuint() const noexcept {
    return vbo;
}

void buffer::bind(GLenum target) const noexcept {
    glBindBuffer(target, vbo);
}

void buffer::swap(buffer& other) noexcept {
    std::swap(vbo, other.vbo);
}

buffer buffer::make() noexcept {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    return buffer(vbo);
}

}