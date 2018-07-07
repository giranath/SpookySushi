add_library(sushi-sdl
        event.cpp
        event.hpp
        window.cpp
        window.hpp
        display.cpp
        display.hpp)
#target_include_directories(sushi-sdl
#        PUBLIC ${SDL2_INCLUDE_DIRS})
#add_dependencies(sushi-sdl SDL2)
target_link_libraries(sushi-sdl PUBLIC libSDL2)