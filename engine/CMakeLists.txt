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
add_subdirectory(input)
add_subdirectory(physic)

add_library(sushi-engine
        application/game_loop.cpp
        application/game_loop.hpp
        application/base_game.hpp application/window_config.cpp application/window_config.hpp application/game_loop_config.cpp application/game_loop_config.hpp application/job_config.cpp application/job_config.hpp)
target_link_libraries(sushi-engine
        PUBLIC sushi-network
        PUBLIC sushi-opengl
        PUBLIC sushi-input
        PUBLIC sushi-sdl
        PUBLIC sushi-async
        PUBLIC sushi-service
        PUBLIC sushi-ecs
        PUBLIC sushi-debug
        PUBLIC sushi-rendering
        PUBLIC sushi-core
        PUBLIC sushi-drivers
        PUBLIC sushi-physic
        PUBLIC libRapidxml
        PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(sushi-engine
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})