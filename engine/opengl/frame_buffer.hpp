#ifndef MMAP_DEMO_FRAME_BUFFER_HPP
#define MMAP_DEMO_FRAME_BUFFER_HPP

#include <GL/gl3w.h>
#include <SDL_opengl.h>

#include "bind.hpp"

namespace sushi { namespace gl {
class frame_buffer {
    GLuint id;

    frame_buffer(GLuint id) noexcept;
public:
    static const frame_buffer SCREEN;

    frame_buffer() noexcept;

    frame_buffer(const frame_buffer&) = delete;
    frame_buffer& operator=(const frame_buffer&) = delete;

    frame_buffer(frame_buffer&& other) noexcept;
    frame_buffer& operator=(frame_buffer&& other) noexcept;

    ~frame_buffer();

    bool good() const noexcept;
    operator GLuint() const noexcept;

    void bind(GLenum target) const noexcept;
    void swap(frame_buffer& other) noexcept;

    static frame_buffer make() noexcept;
};

template<GLenum TARGET = GL_FRAMEBUFFER>
class framebuffer_bind {
    const frame_buffer& buf;
public:
    explicit framebuffer_bind(const frame_buffer& buf)
    : buf{buf} {

    }

    void bind() const noexcept {
        buf.bind(TARGET);
    }
};

template<GLenum TARGET>
struct is_bindable<framebuffer_bind<TARGET>> {
    static const bool value = true;
};

}}

#endif //MMAP_DEMO_FRAME_BUFFER_HPP
