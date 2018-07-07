add_library(sushi-opengl
        ${GL3W_SOURCES}
        bind.hpp
        buffer.cpp
        buffer.hpp
        debug.cpp
        debug.hpp
        frame_buffer.cpp
        frame_buffer.hpp
        opengl.hpp
        program.cpp
        program.hpp
        render_buffer.cpp
        render_buffer.hpp
        shader.cpp
        shader.hpp
        texture.cpp
        texture.hpp
        uniform.hpp
        vertex_array.cpp
        vertex_array.hpp)
target_include_directories(sushi-opengl
        PUBLIC ${GL3W_INCLUDE_DIRS})
target_link_libraries(sushi-opengl
        PUBLIC libSDL2
        PUBLIC libGlm)