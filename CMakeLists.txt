cmake_minimum_required(VERSION 3.9)

project(SpookySushi VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# Get dependencies
find_package(SDL2 REQUIRED)

# Game engine's
add_library(spooky-engine
        engine/sdl/window.cpp
        engine/sdl/window.hpp
        engine/sdl/event.cpp
        engine/sdl/event.hpp)
target_link_libraries(spooky-engine
        ${SDL2_LIBRARIES})
target_include_directories(spooky-engine
        PUBLIC ${SDL2_INCLUDE_DIRS})

# Game executable
add_executable(spooky-sushi
        src/main.cpp
        )
target_link_libraries(spooky-sushi
        spooky-engine)
#target_include_directories(spooky-sushi)