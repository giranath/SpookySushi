#include "frame_buffer.hpp"

#include <algorithm>

namespace sushi { namespace gl {

const frame_buffer frame_buffer::SCREEN{};

frame_buffer::frame_buffer() noexcept
: id(0) {

}

frame_buffer::frame_buffer(GLuint id) noexcept
: id(id) {

}

frame_buffer::frame_buffer(frame_buffer&& other) noexcept
: id(other.id){
    other.id = 0;
}

frame_buffer& frame_buffer::operator=(frame_buffer&& other) noexcept {
    std::swap(id, other.id);

    return *this;
}

frame_buffer::~frame_buffer() {
    glDeleteFramebuffers(1, &id);
}

bool frame_buffer::good() const noexcept {
    return id != 0;
}

frame_buffer::operator GLuint() const noexcept {
    return id;
}

void frame_buffer::bind(GLenum target) const noexcept {
    glBindFramebuffer(target, id);
}

void frame_buffer::swap(frame_buffer& other) noexcept {
    std::swap(id, other.id);
}

frame_buffer frame_buffer::make() noexcept {
    GLuint id = 0;

    glGenFramebuffers(1, &id);

    return frame_buffer(id);
}

}}