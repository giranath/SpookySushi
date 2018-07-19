#include "render_buffer.hpp"
#include <algorithm>

namespace sushi { namespace gl {

render_buffer::render_buffer(GLuint id)
: id{id} {

}

render_buffer::render_buffer(render_buffer&& other) noexcept
: id(other.id){
    other.id = 0;
}

render_buffer& render_buffer::operator=(render_buffer&& other) noexcept {
    std::swap(id, other.id);
    return *this;
}

render_buffer::~render_buffer() {
    glDeleteRenderbuffers(1, &id);
}

bool render_buffer::good() const noexcept {
    return id != 0;
}

render_buffer::operator GLuint() const noexcept {
    return id;
}

void render_buffer::bind(GLenum target) const noexcept {
    glBindRenderbuffer(target, id);
}

void render_buffer::swap(render_buffer& other) noexcept {
    std::swap(id, other.id);
}

render_buffer render_buffer::make() noexcept {
    GLuint id;
    glGenRenderbuffers(1, &id);

    return render_buffer(id);
}

}}