add_library(sushi-sdl
        event.cpp
        event.hpp
        window.cpp
        window.hpp
        display.cpp
        display.hpp)
target_link_libraries(sushi-sdl PUBLIC libSDL2)