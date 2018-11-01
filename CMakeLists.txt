cmake_minimum_required(VERSION 3.9)

project(SpookySushi VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(MSVC)

# Project configurations
set(SUSHI_CACHE_LINE_SIZE 64 CACHE STRING "Specify the cache line size")
option(BUILD_TOOLS "Build all tools when enabled" OFF)
option(ENABLE_EDITOR "Enable editor related functionalities" OFF)
option(ENABLE_PROFILING "Enable code profiling" OFF)

include(cmake/detect_std_support.cmake)
include(cmake/detect_kernel_support.cmake)
include(cmake/dependencies.cmake)

set(SUSHI_RENDERING_BACKEND "OpenGL" CACHE STRING "Specify the rendering backend to use")
set_property(CACHE SUSHI_RENDERING_BACKEND PROPERTY STRINGS OpenGL)
set(SUSHI_PHYSIC_BACKEND "Bullet" CACHE STRING "Specify the physic backend to use")
set_property(CACHE SUSHI_PHYSIC_BACKEND PROPERTY STRINGS Bullet PhysX None)

# Find CMake official packages
find_package(Threads REQUIRED)

enable_testing()

add_subdirectory(engine)
if(BUILD_TOOLS)
    add_subdirectory(tool)
endif()

# Game executable
add_executable(spooky-sushi
        src/game.cpp
        src/game.hpp
        src/main.cpp
        src/game_controller.cpp
        src/game_controller.hpp src/camera_controller.cpp src/camera_controller.hpp)
target_link_libraries(spooky-sushi
        PRIVATE sushi-engine)