#ifndef MMAP_DEMO_PROGRAM_HPP
#define MMAP_DEMO_PROGRAM_HPP

// Theses includes must be first
#include <GL/gl3w.h>
#include <SDL_opengl.h>

#include "shader.hpp"
#include "uniform.hpp"
#include "bind.hpp"

#include <memory>
#include <string>
#include <algorithm>

namespace gl {

template<typename ptr>
struct is_pointer_like {
    static const bool value = false;
};

template<typename ptr>
struct is_pointer_like<ptr*> {
    static const bool value = true;
};

template<typename ptr>
struct is_pointer_like<const ptr*> {
    static const bool value = true;
};

template<typename ptr>
struct is_pointer_like<std::unique_ptr<ptr>> {
    static const bool value = true;
};

template<typename ptr>
struct is_pointer_like<std::shared_ptr<ptr>> {
    static const bool value = true;
};

class program {
    GLuint prog;
public:
    class link_status {
        bool is_linked;
        std::string msg;
    public:
        link_status(bool is_linked, const std::string& msg);

        bool good() const noexcept;
        operator bool() const noexcept;

        const std::string& message() const noexcept;
    };

    program() noexcept;

    program(const program&) = delete;
    program& operator=(const program&) = delete;

    program(program&&) noexcept;
    program& operator=(program&&) noexcept;

    ~program() noexcept;

    template<typename SHADER>
    void attach(const SHADER& shader) const noexcept {
        static_assert(is_shader<SHADER>::value, "you must specify a shader type");

        glAttachShader(prog, shader);
    }

    template<typename it>
    void attach(it begin, it end) const noexcept {
        using iterator_type = typename std::iterator_traits<it>::value_type;

        static_assert(is_shader<iterator_type>::value
                      || (is_pointer_like<iterator_type>::value
                          && is_shader<typename std::pointer_traits<iterator_type>::element_type>::value), "unsupported iterator type");

        // If iterator on shaders
        if constexpr(is_shader<iterator_type>::value) {
            std::for_each(begin, end, [this](const gl::shader& shader) {
                attach(shader);
            });
        }
            // If iterator on pointer of shaders
        else if constexpr(is_pointer_like<iterator_type>::value
                          && is_shader<typename std::pointer_traits<iterator_type>::element_type>::value) {
            std::for_each(begin, end, [this](const auto& ptr) {
                attach(*ptr);
            });
        }
    }

    operator GLuint() const noexcept;

    link_status link() const noexcept;

    void bind() const noexcept;

    template<typename T>
    uniform<T> find_uniform(const char* name) const noexcept {
        GLint uniform_location = glGetUniformLocation(prog, name);

        if(uniform_location >= 0) {
            return uniform<T>{static_cast<GLuint>(uniform_location)};
        }

        return uniform<T>{};
    }
};

template<>
struct is_bindable<program> {
    static const bool value = true;
};

}


#endif
