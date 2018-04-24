#include "shader.hpp"

namespace gl {

shader::shader(GLenum type) noexcept
: raw(glCreateShader(type)){

}

shader::shader(shader&& other) noexcept
: raw(other.raw){
    other.raw = 0;
}

shader& shader::operator=(shader&& other) noexcept {
    std::swap(raw, other.raw);

    return *this;
}

shader::~shader() noexcept {
    glDeleteShader(raw);
}

shader::compilation_status shader::compile(std::istream& source_stream) const noexcept {
    std::string source;
    char letter;
    while(source_stream.get(letter)) {
        source.push_back(letter);
    }

    return compile(source);
}

shader::compilation_status shader::compile(const char* source) const noexcept {
    // Load the sources into the shader
    glShaderSource(raw, 1, &source, nullptr);

    // Compile the shader
    glCompileShader(raw);
    GLint is_compiled;
    glGetShaderiv(raw, GL_COMPILE_STATUS, &is_compiled);

    GLsizei log_len = 0;
    glGetShaderiv(raw, GL_INFO_LOG_LENGTH, &log_len);

    std::string message;
    if(log_len > 0) {
        GLchar *message_buffer = new(std::nothrow) GLchar[log_len];
        std::fill(message_buffer, message_buffer + log_len, 0);
        if (message_buffer) {
            glGetShaderInfoLog(raw, log_len, nullptr, message_buffer);
            message = std::string{message_buffer};
            delete[] message_buffer;
        }
    }

    return compilation_status{is_compiled == GL_FALSE, message};
}

shader::compilation_status shader::compile(const std::string& source) const noexcept {
    return compile(source.c_str());
}

bool shader::good() const noexcept {
    return raw != 0;
}

shader::operator GLuint() const noexcept {
    return raw;
}

}