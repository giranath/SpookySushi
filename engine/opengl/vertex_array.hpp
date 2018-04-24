#ifndef MMAP_DEMO_VERTEX_ARRAY_HPP
#define MMAP_DEMO_VERTEX_ARRAY_HPP

// Theses includes must be first
#include <GL/gl3w.h>
#include <SDL_opengl.h>

#include "bind.hpp"

namespace gl {
class vertex_array {
    GLuint vao {};

    explicit vertex_array(GLuint vao) noexcept;
public:
    // By default, a vertex_array is invalid
    vertex_array() noexcept = default;

    // A vertex array can be moved
    vertex_array(vertex_array&&) noexcept;
    vertex_array& operator=(vertex_array&&) noexcept;

    // A vertex array cannot be copied
    vertex_array(const vertex_array&) = delete;
    vertex_array& operator=(const vertex_array&) = delete;

    ~vertex_array() noexcept;

    bool good() const noexcept;
    operator GLuint() const noexcept;

    void bind() const noexcept;
    void swap(vertex_array& other) noexcept;

    // Factory function to create valid vao
    static vertex_array make() noexcept;
};

template<>
struct is_bindable<vertex_array> {
    static const bool value = true;
};
}

#endif