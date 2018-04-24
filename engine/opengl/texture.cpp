#include "texture.hpp"

#include <algorithm>

#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

namespace gl {

texture::texture() noexcept
: raw{0} {

}

texture::texture(GLuint raw) noexcept
: raw{raw} {

}

texture::texture(texture&& other) noexcept
: raw(other.raw) {
    other.raw = 0;
}

texture::~texture() noexcept {
    glDeleteTextures(1, &raw);
}

texture& texture::operator=(texture&& other) noexcept {
    std::swap(raw, other.raw);

    return *this;
}

void texture::swap(texture& other) noexcept {
    std::swap(raw, other.raw);
}

texture::operator GLuint() const noexcept {
    return raw;
}

void texture::bind(GLenum target) const noexcept {
    glBindTexture(target, raw);
}

bool texture::good() const noexcept {
    return raw != 0;
}

texture texture::make() noexcept {
    GLuint text;
    glGenTextures(1, &text);

    return texture{text};
}

texture texture::make(uint32_t width, uint32_t height) noexcept {
    GLuint tex;
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return texture{tex};
}

texture texture::load_from_path(const char* path) noexcept {
    SDL_Surface* surface = IMG_Load(path);
    if(surface) {
        texture tex = make();

        int mode = GL_RGB;
        if(surface->format->BytesPerPixel == 4) {
            mode = GL_RGBA;
        }

        tex.bind(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return tex;
    }

    return texture{};
}

}