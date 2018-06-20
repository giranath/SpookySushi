add_library(sushi-rendering
        renderer_interface.hpp
        proxy_renderer.cpp
        proxy_renderer.hpp
        static_mesh_builder.cpp
        static_mesh_builder.hpp)

target_link_libraries(sushi-rendering
        PRIVATE sushi-sdl)

if(SUSHI_RENDERING_BACKEND STREQUAL "OpenGL")
    target_sources(sushi-rendering PRIVATE
            opengl_renderer.cpp
            opengl_renderer.hpp)
    target_link_libraries(sushi-rendering
            PRIVATE sushi-opengl)
endif()