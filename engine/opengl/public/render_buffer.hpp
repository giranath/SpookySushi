#ifndef MMAP_DEMO_RENDER_BUFFER_HPP
#define MMAP_DEMO_RENDER_BUFFER_HPP

#include "private_opengl.hpp"

#include "bind.hpp"

namespace sushi { namespace gl {

class render_buffer {
    GLuint id {};

    render_buffer(GLuint id);
public:
    render_buffer() noexcept = default;

    render_buffer(const render_buffer&) = delete;
    render_buffer& operator=(const render_buffer&) = delete;

    render_buffer(render_buffer&& other) noexcept;
    render_buffer& operator=(render_buffer&& other) noexcept;

    ~render_buffer();

    bool good() const noexcept;
    operator GLuint() const noexcept;

    void bind(GLenum target) const noexcept;
    void swap(render_buffer& other) noexcept;

    static render_buffer make() noexcept;
};

template<GLenum TARGET>
class renderbuffer_bind {
    const render_buffer& buf;
public:
    explicit renderbuffer_bind(const render_buffer& buf)
    : buf{buf} {

    }

    void bind() const noexcept {
        buf.bind(TARGET);
    }
};

template<GLenum TARGET>
struct is_bindable<renderbuffer_bind<TARGET>> {
    static const bool value = true;
};

}}

#endif //MMAP_DEMO_RENDER_BUFFER_HPP
