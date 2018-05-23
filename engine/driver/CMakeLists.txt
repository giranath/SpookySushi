add_subdirectory(socket)
set(SUSHI_DRIVER_SOCKET_NAME "${SUSHI_DRIVER_SOCKET_NAME}" PARENT_SCOPE)

add_library(sushi-drivers
        $<TARGET_OBJECTS:sushi-driver-socket>
        )
target_link_libraries(sushi-drivers
        PUBLIC ${DRIVER_SOCKET_TARGET_LIBRARIES}
        )