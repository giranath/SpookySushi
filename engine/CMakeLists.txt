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
add_subdirectory(plugin)

add_library(sushi-engine
        application/private/game_loop.cpp
        application/public/game_loop.hpp
        application/public/base_game.hpp
        application/private/window_config.cpp
        application/public/window_config.hpp
        application/private/game_loop_config.cpp
        application/public/game_loop_config.hpp
        application/private/job_config.cpp
        application/public/job_config.hpp)
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
        PUBLIC sushi-plugin
        PUBLIC libRapidxml
        PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(sushi-engine
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC application/public
        PRIVATE application/private)