#ifndef MMAP_DEMO_TEXTURE_H
#define MMAP_DEMO_TEXTURE_H

#include <GL/gl3w.h>
#include <SDL_opengl.h>
#include <fstream>

#include "bind.hpp"

namespace sushi { namespace gl {

class texture {
    GLuint raw;
    explicit texture(GLuint raw) noexcept;
public:
    texture() noexcept;
    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;
    texture(texture&& other) noexcept;
    texture& operator=(texture&& other) noexcept;
    ~texture() noexcept;

    void swap(texture& other) noexcept;

    operator GLuint() const noexcept;

    void bind(GLenum target) const noexcept;

    bool good() const noexcept;

    static texture make() noexcept;
    static texture make(uint32_t width, uint32_t height) noexcept;
    static texture load_from_path(const char* path) noexcept;
};

template<GLenum TARGET>
class texture_bind {
    const texture& tex;
public:
    explicit texture_bind(const texture& tex)
    : tex{tex} {

    }

    void bind() const noexcept {
        tex.bind(TARGET);
    }
};

template<GLenum TARGET>
struct is_bindable<texture_bind<TARGET>> {
    static const bool value = true;
};

}}

#endif
