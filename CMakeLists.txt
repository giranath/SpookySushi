cmake_minimum_required(VERSION 3.9)

project(SpookySushi VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# Get dependencies
find_package(SDL2 REQUIRED)

# Game executable
add_executable(spooky-sushi
        src/main.cpp
        )
target_link_libraries(spooky-sushi
        ${SDL2_LIBRARIES})
target_include_directories(spooky-sushi
        PRIVATE ${SDL2_INCLUDE_DIRS})