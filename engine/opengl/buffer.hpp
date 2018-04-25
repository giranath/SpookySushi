#ifndef MMAP_DEMO_BUFFER_HPP
#define MMAP_DEMO_BUFFER_HPP

// Theses includes must be first
#include <GL/gl3w.h>
#include <SDL_opengl.h>

#include "bind.hpp"

namespace sushi { namespace gl {

class buffer {
    GLuint vbo{};

    explicit buffer(GLuint vbo) noexcept;

public:
    buffer() noexcept = default;

    buffer(const buffer &) = delete;

    buffer &operator=(const buffer &) = delete;

    buffer(buffer &&) noexcept;

    buffer &operator=(buffer &&) noexcept;

    ~buffer() noexcept;

    bool good() const noexcept;

    operator GLuint() const noexcept;

    void bind(GLenum target) const noexcept;

    void swap(buffer &other) noexcept;

    static buffer make() noexcept;
};

template<GLenum TARGET>
class buffer_bind {
    const buffer &buf;
public:
    explicit buffer_bind(const buffer &buf)
            : buf{buf} {

    }

    void bind() const noexcept {
        buf.bind(TARGET);
    }
};

template<GLenum TARGET>
struct is_bindable<buffer_bind<TARGET>> {
    static const bool value = true;
};

}}

#endif
