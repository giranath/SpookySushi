add_library(sushi-rendering
        renderer_interface.hpp
        opengl_renderer.cpp
        opengl_renderer.hpp proxy_renderer.cpp proxy_renderer.hpp mesh.cpp mesh.hpp)
target_link_libraries(sushi-rendering
        PRIVATE sushi-sdl
        PRIVATE sushi-opengl)