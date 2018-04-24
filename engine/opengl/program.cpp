#include "program.hpp"

namespace gl {

program::link_status::link_status(bool is_linked, const std::string& msg)
: is_linked{is_linked}
, msg{msg} {

}

bool program::link_status::good() const noexcept {
    return is_linked;
}

program::link_status::operator bool() const noexcept {
    return good();
}

const std::string& program::link_status::message() const noexcept {
    return msg;
}

program::program() noexcept
: prog{glCreateProgram()}{
}

program::program(program&& other) noexcept
: prog{other.prog} {
    other.prog = 0;
}

program& program::operator=(program&& other) noexcept {
    std::swap(prog, other.prog);

    return *this;
}

program::~program() noexcept {
    glDeleteProgram(prog);
}

program::operator GLuint() const noexcept {
    return prog;
}

program::link_status program::link() const noexcept {
    glLinkProgram(prog);

    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);

    GLsizei log_len = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &log_len);

    std::string message;
    GLchar* message_buffer = new(std::nothrow) GLchar[log_len];
    if(message_buffer) {
        glGetProgramInfoLog(prog, log_len, nullptr, message_buffer);
        message = std::string{message_buffer};
        delete[] message_buffer;
    }

    return link_status{status == GL_TRUE, ""};
}

void program::bind() const noexcept {
    glUseProgram(prog);
}

}