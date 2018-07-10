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
include(cmake/detect_std_support.cmake)
include(cmake/detect_kernel_support.cmake)
include(cmake/dependencies.cmake)

set(SUSHI_RENDERING_BACKEND "OpenGL" CACHE STRING "Specify the rendering backend to use")
set_property(CACHE SUSHI_RENDERING_BACKEND PROPERTY STRINGS OpenGL)

# Find CMake official packages
find_package(Threads REQUIRED)

add_subdirectory(engine)

# Game executable
add_executable(spooky-sushi
        src/game.cpp
        src/game.hpp
        src/main.cpp
        )
target_link_libraries(spooky-sushi
        PRIVATE sushi-engine
        PRIVATE libSDL2main)