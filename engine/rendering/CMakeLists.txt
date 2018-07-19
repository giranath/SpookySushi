add_library(sushi-rendering
        renderer_interface.hpp
        opengl_renderer.cpp
        opengl_renderer.hpp
        proxy_renderer.cpp
        proxy_renderer.hpp
        mesh.cpp
        mesh.hpp
        mesh_builder.cpp
        mesh_builder.hpp
        proxy_renderer.cpp
        proxy_renderer.hpp
        static_mesh_builder.cpp
        static_mesh_builder.hpp
        camera.cpp
        camera.hpp transform.cpp transform.hpp)

target_link_libraries(sushi-rendering
        PRIVATE sushi-sdl
        PUBLIC sushi-core)

if(SUSHI_RENDERING_BACKEND STREQUAL "OpenGL")
    target_sources(sushi-rendering PRIVATE
            opengl_renderer.cpp
            opengl_renderer.hpp)
    target_link_libraries(sushi-rendering
            PRIVATE sushi-opengl)
endif()