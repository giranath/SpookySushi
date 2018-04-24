#include "debug.hpp"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

namespace gl {

std::ostream& print_opengl_severity(std::ostream& os, GLenum severity) {
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            os << "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            os << "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW_ARB:
            os << "low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            os << "debug";
            break;
        default:
            os << "unknown";
            break;
    }

    return os;
}

std::ostream& print_opengl_type(std::ostream& os, GLenum type) {
    switch(type) {
        case GL_DEBUG_TYPE_ERROR_ARB:
            os << "error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            os << "deprecated";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            os << "undefined behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
            os << "portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
            os << "performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            os << "marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            os << "push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            os << "pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            os << "other";
            break;
        default:
            os << "unknown";
            break;
    }
    return os;
}

std::ostream& print_opengl_source(std::ostream& os, GLenum source) {
    switch(source) {
        case GL_DEBUG_SOURCE_API:
            os << "api";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            os << "window";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            os << "compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            os << "third-party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
            os << "application";
            break;
        case GL_DEBUG_SOURCE_OTHER_ARB:
            os << "other";
            break;
        default:
            os << "unknown";
            break;
    }
    return os;
}

void opengl_message_cb(GLenum source, GLenum type, GLuint id, GLenum severity,
                       GLsizei /*len*/, const GLchar* message, const void* /*user_params*/) {
    std::cerr << "[GL:" << id << "]";

    std::cerr << "[";
    print_opengl_source(std::cerr, source);
    std::cerr << "]";

    std::cerr << "[";
    print_opengl_severity(std::cerr, severity);
    std::cerr << "]";

    std::cerr << "[";
    print_opengl_type(std::cerr, type);
    std::cerr << "] ";

    std::cerr << message << std::endl;
}

bool enable_debug_messages() noexcept {
    bool is_ok = true;
    // We check if glDebugMessageCallback exists
    GL3WglProc debug_msg_callback_extension = gl3wGetProcAddress("glDebugMessageCallbackARB");
    if(debug_msg_callback_extension) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

        typedef GLvoid (APIENTRYP DebugMessageCallbackARBPROC)(GLDEBUGPROCARB, GLvoid*);
        auto glDebugMessageCallbackARB = reinterpret_cast<DebugMessageCallbackARBPROC>(debug_msg_callback_extension);

        glDebugMessageCallbackARB((GLDEBUGPROCARB) opengl_message_cb, nullptr);
    }
    else {
        is_ok = false;
    }

    GL3WglProc debug_msg_control_extension = gl3wGetProcAddress("glDebugMessageControl");
    if(debug_msg_control_extension) {
        typedef GLvoid (APIENTRYP DebugMessageControlARBPROC)(GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean);
        auto glDebugMessageControlARB = reinterpret_cast<DebugMessageControlARBPROC>(debug_msg_control_extension);

        // Disable nvidia
        GLuint ids_to_disable[] = { 131185 };
        glDebugMessageControlARB(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, sizeof(ids_to_disable), &ids_to_disable[0], GL_FALSE);
    }
    else {
        is_ok = false;
    }

    return is_ok;
}

}