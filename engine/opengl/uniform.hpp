#ifndef MMAP_DEMO_UNIFORM_HPP
#define MMAP_DEMO_UNIFORM_HPP

#include <GL/gl3w.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl {

struct scalar_tag {};
struct vector_tag {};
struct matrix_tag {};
struct unspecified_tag {};

template<typename T>
struct uniform_traits {
    using value_type = T;
    using category = unspecified_tag;

    static const bool is_uniform = false;
    static void set(GLuint uniform, const value_type& v) {}
};

// Integers
template<>
struct uniform_traits<int> {
    using value_type = int;
    using category = scalar_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type v) {
        glUniform1i(uniform, v);
    }
};

// Floats
template<>
struct uniform_traits<float> {
    using value_type = float;
    using category = scalar_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type v) {
        glUniform1f(uniform, v);
    }
};

template<>
struct uniform_traits<glm::vec2> {
    using value_type = glm::vec2;
    using category = vector_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type vec) {
        glUniform2f(uniform, vec.x, vec.y);
    }
};

template<>
struct uniform_traits<glm::vec3> {
    using value_type = glm::vec3;
    using category = vector_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type vec) {
        glUniform3f(uniform, vec.x, vec.y, vec.z);
    }
};

template<>
struct uniform_traits<glm::vec4> {
    using value_type = glm::vec4;
    using category = vector_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type vec) {
        glUniform4f(uniform, vec.x, vec.y, vec.z, vec.w);
    }
};

// Matrices
template<>
struct uniform_traits<glm::mat2> {
    using value_type = glm::mat2;
    using category = matrix_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type vec, bool should_transpose) {
        glUniformMatrix2fv(uniform, 1, static_cast<GLboolean>(should_transpose ? GL_TRUE : GL_FALSE), glm::value_ptr(vec));
    }
};

template<>
struct uniform_traits<glm::mat3> {
    using value_type = glm::mat3;
    using category = matrix_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type vec, bool should_transpose) {
        glUniformMatrix3fv(uniform, 1, static_cast<GLboolean>(should_transpose ? GL_TRUE : GL_FALSE), glm::value_ptr(vec));
    }
};

template<>
struct uniform_traits<glm::mat4> {
    using value_type = glm::mat4;
    using category = matrix_tag;

    static const bool is_uniform = true;
    static void set(GLuint uniform, value_type vec, bool should_transpose) {
        glUniformMatrix4fv(uniform, 1, static_cast<GLboolean>(should_transpose ? GL_TRUE : GL_FALSE), glm::value_ptr(vec));
    }
};

class program;

template<typename T>
class uniform {
    static_assert(uniform_traits<T>::is_uniform, "you must specify a valid uniform type");
    friend program;

    GLuint raw {};

    explicit uniform(GLuint value)
    : raw{value} {

    }

    void set(T value, matrix_tag) {
        uniform_traits<T>::set(raw, value, false);
    }

    void set(T value, vector_tag) {
        uniform_traits<T>::set(raw, value);
    }

    void set(T value, scalar_tag) {
        uniform_traits<T>::set(raw, value);
    }

public:
    uniform() = default;

    bool good() const noexcept {
        return raw != 0;
    }

    void set(T value) noexcept {
        set(value, typename uniform_traits<T>::category{});
    }
};

}

#endif //MMAP_DEMO_UNIFORM_HPP
