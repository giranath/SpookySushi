add_subdirectory(socket)
set(SUSHI_DRIVER_SOCKET_NAME "${SUSHI_DRIVER_SOCKET_NAME}" PARENT_SCOPE)

add_subdirectory(dynamic_linking)
set(SUSHI_DRIVER_DYNAMIC_LINKING_NAME "${SUSHI_DRIVER_DYNAMIC_LINKING_NAME}" PARENT_SCOPE)

add_library(sushi-drivers
        $<TARGET_OBJECTS:sushi-driver-socket>
        $<TARGET_OBJECTS:sushi-driver-dynamic-linking>
        )
target_link_libraries(sushi-drivers
        PUBLIC ${DRIVER_SOCKET_TARGET_LIBRARIES}
        PUBLIC ${DRIVER_DYNAMIC_LINKING_TARGET_LIBRARIES}
        )