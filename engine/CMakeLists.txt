get_target_property(GL3W_INCLUDE_DIRS gl3w INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(GL3W_SOURCES gl3w INTERFACE_SOURCES)

add_subdirectory(driver)
add_subdirectory(platform)
add_subdirectory(network)
add_subdirectory(core)
add_subdirectory(opengl)
add_subdirectory(sdl)
add_subdirectory(async)
add_subdirectory(service)
add_subdirectory(ecs)
add_subdirectory(debug)
add_subdirectory(rendering)

add_library(sushi-engine
        application/game_loop.cpp
        application/game_loop.hpp
        application/base_game.hpp
        core/string_utils.hpp)
target_link_libraries(sushi-engine
        PUBLIC sushi-network
        PUBLIC sushi-opengl
        PUBLIC sushi-sdl
        PUBLIC sushi-async
        PUBLIC sushi-service
        PUBLIC sushi-ecs
        PUBLIC sushi-debug
        PUBLIC sushi-rendering
        PUBLIC sushi-drivers
        PRIVATE ${CMAKE_THREAD_LIBS_INIT}
        PRIVATE ${SDL2_LIBRARIES}
        PRIVATE gl3w)
target_include_directories(sushi-engine
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PUBLIC ${GL3W_INCLUDE_DIRS}
        PUBLIC ${TOML11_INCLUDE_DIRS}
        )