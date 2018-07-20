add_library(sushi-sdl
        private/sdl_event.cpp
        public/sdl_event.hpp
        private/window.cpp
        public/window.hpp
        private/display.cpp
        public/display.hpp)
target_link_libraries(sushi-sdl
        PUBLIC libSDL2
        INTERFACE libSDL2main
        PUBLIC sushi-core)
target_include_directories(sushi-sdl
        PUBLIC public
        PRIVATE private)