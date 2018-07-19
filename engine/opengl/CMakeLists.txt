add_library(sushi-opengl
        public/bind.hpp
        private/buffer.cpp
        public/buffer.hpp
        private/debug.cpp
        public/debug.hpp
        private/frame_buffer.cpp
        public/frame_buffer.hpp
        public/opengl.hpp
        private/program.cpp
        public/program.hpp
        private/render_buffer.cpp
        public/render_buffer.hpp
        private/shader.cpp
        public/shader.hpp
        private/texture.cpp
        public/texture.hpp
        public/uniform.hpp
        private/vertex_array.cpp
        public/vertex_array.hpp
        public/private_opengl.hpp)
target_link_libraries(sushi-opengl
        PUBLIC libSDL2
        PUBLIC libGlm
        PUBLIC libGlad)
target_include_directories(sushi-opengl
        PUBLIC public
        PRIVATE private)