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
        public/transform.hpp
        private/opengl_static_mesh.cpp
        private/opengl_static_mesh.hpp
        private/open_gl_debug_renderer.cpp
        private/open_gl_debug_renderer.hpp
        private/opengl_debug_shape_renderer.hpp
        public/debug_shapes.hpp
        public/debug_shape_traits.hpp
        private/debug_shapes.cpp
        public/camera_len.hpp
        private/perspective_camera_len.cpp)

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