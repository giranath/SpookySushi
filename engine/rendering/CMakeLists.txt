add_library(sushi-rendering
        public/renderer_interface.hpp
        private/opengl_renderer.cpp
        public/opengl_renderer.hpp
        private/proxy_renderer.cpp
        public/proxy_renderer.hpp
        private/mesh.cpp
        public/mesh.hpp
        private/mesh_builder.cpp
        public/mesh_builder.hpp
        private/proxy_renderer.cpp
        public/proxy_renderer.hpp
        private/static_mesh_builder.cpp
        public/static_mesh_builder.hpp
        private/camera.cpp
        public/camera.hpp
        private/transform.cpp
        public/transform.hpp)

target_link_libraries(sushi-rendering
        PRIVATE sushi-sdl
        PUBLIC sushi-core
        PRIVATE sushi-service)
target_include_directories(sushi-rendering
        PUBLIC public
        PRIVATE private)
target_compile_definitions(sushi-rendering
        PRIVATE GLM_ENABLE_EXPERIMENTAL)

if(SUSHI_RENDERING_BACKEND STREQUAL "OpenGL")
    target_sources(sushi-rendering PRIVATE
            private/opengl_renderer.cpp
            public/opengl_renderer.hpp)
    target_link_libraries(sushi-rendering
            PRIVATE sushi-opengl)
endif()