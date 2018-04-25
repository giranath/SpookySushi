#ifndef MMAP_DEMO_SHADER_HPP
#define MMAP_DEMO_SHADER_HPP

// Theses includes must be first
#include <GL/gl3w.h>
#include <SDL_opengl.h>

#include <iostream>
#include <string>

namespace sushi { namespace gl {
struct vertex_shader_type {
    static const GLenum value = GL_VERTEX_SHADER;
};
struct fragment_shader_type {
    static const GLenum value = GL_FRAGMENT_SHADER;
};

template<typename T>
struct is_shader_type {
    static const bool value = false;
};

template<>
struct is_shader_type<vertex_shader_type> {
    static const bool value = true;
};

template<>
struct is_shader_type<fragment_shader_type> {
    static const bool value = true;
};

class shader {
    GLuint raw;
protected:
    shader(GLenum type) noexcept;
public:
    class compilation_status {
        std::string msg;
        bool has_failed;
    public:
        compilation_status(bool has_failed, const std::string& msg)
        : msg{msg}
        , has_failed{has_failed} {

        }

        bool good() const noexcept {
            return !has_failed;
        }

        operator bool() const noexcept {
            return good();
        }

        const std::string& message() const noexcept {
            return msg;
        }
    };

    // Copy is disabled
    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    // Move is enabled
    shader(shader&&) noexcept;
    shader& operator=(shader&&) noexcept;

    virtual ~shader() noexcept;

    compilation_status compile(const char* source) const noexcept;
    compilation_status compile(const std::string& source) const noexcept;
    compilation_status compile(std::istream& source_stream) const noexcept;

    bool good() const noexcept;

    operator GLuint() const noexcept;
};

template<typename TYPE>
class typed_shader : public shader {
    static_assert(is_shader_type<TYPE>::value, "you must specify a valid shader type");
public:
    using type = TYPE;

    typed_shader() noexcept
            : shader(TYPE::value) {

    }
};

//======================================================================================================================
// Types
//======================================================================================================================
using vertex_shader = typed_shader<vertex_shader_type>;
using fragment_shader = typed_shader<fragment_shader_type>;

//======================================================================================================================
// Traits
//======================================================================================================================
template<typename T>
struct is_shader {
    static const bool value = false;
};

template<>
struct is_shader<shader> {
    static const bool value = true;
};

template<>
struct is_shader<const shader> {
    static const bool value = true;
};

template<>
struct is_shader<shader&> {
    static const bool value = true;
};

template<>
struct is_shader<const shader&> {
    static const bool value = true;
};

template<typename T>
struct is_shader<typed_shader<T>> {
    static const bool value = true;
};

template<typename T>
struct is_shader<const typed_shader<T>> {
    static const bool value = true;
};

template<typename T>
struct is_shader<typed_shader<T>&> {
    static const bool value = true;
};

template<typename T>
struct is_shader<const typed_shader<T>&> {
    static const bool value = true;
};

}}

#endif
