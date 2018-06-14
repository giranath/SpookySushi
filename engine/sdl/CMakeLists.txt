add_library(sushi-sdl
        event.cpp
        event.hpp
        window.cpp
        window.hpp
        display.cpp
        display.hpp)
target_include_directories(sushi-sdl
        PUBLIC ${SDL2_INCLUDE_DIRS})