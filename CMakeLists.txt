cmake_minimum_required(VERSION 3.9)

project(SpookySushi VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)

# Project configurations
set(SUSHI_CACHE_LINE_SIZE 64 CACHE STRING "Specify the cache line size")

# Get dependencies
find_package(SDL2 REQUIRED)
find_package(Toml11 REQUIRED)
find_package(gl3w REQUIRED)
find_package(glm REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(engine)

# Game executable
add_executable(spooky-sushi
        src/game_loop.hpp
        src/main.cpp
        )
target_link_libraries(spooky-sushi
        sushi-engine)