#ifndef MMAP_DEMO_DEBUG_HPP
#define MMAP_DEMO_DEBUG_HPP

// Theses includes must be first
#include <GL/gl3w.h>
#include <SDL_opengl.h>
#include <iostream>

namespace gl {

bool enable_debug_messages() noexcept;

template<typename OutIt>
void get_extensions(OutIt iterator) noexcept {
    GLint extension_count = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);

    for(GLint i = 0; i < extension_count; ++i) {
        const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        *iterator = extension;
        ++iterator;
    }
}

}

#endif
