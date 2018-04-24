add_library(sushi-sdl OBJECT
        event.cpp
        event.hpp
        window.cpp
        window.hpp)
target_include_directories(sushi-sdl
        PUBLIC ${SDL2_INCLUDE_DIRS})