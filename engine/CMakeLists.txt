get_target_property(GL3W_INCLUDE_DIRS gl3w INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(GL3W_SOURCES gl3w INTERFACE_SOURCES)

add_subdirectory(platform)
add_subdirectory(opengl)
add_subdirectory(sdl)
add_subdirectory(async)
add_subdirectory(service)
add_subdirectory(debug)

add_library(sushi-engine
        $<TARGET_OBJECTS:sushi-debug>
        $<TARGET_OBJECTS:sushi-service>
        $<TARGET_OBJECTS:sushi-opengl>
        $<TARGET_OBJECTS:sushi-sdl>
        $<TARGET_OBJECTS:sushi-async>
        $<TARGET_OBJECTS:sushi-platform>)
target_link_libraries(sushi-engine
        ${CMAKE_THREAD_LIBS_INIT}
        ${SDL2_LIBRARIES}
        gl3w)
target_include_directories(sushi-engine
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PUBLIC ${GL3W_INCLUDE_DIRS}
        PUBLIC ${TOML11_INCLUDE_DIRS}
        )