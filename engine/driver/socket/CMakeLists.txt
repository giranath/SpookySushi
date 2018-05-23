set(SUSHI_HAS_SOCKET_SUPPORT ON PARENT_SCOPE)
set(DRIVER_SOCKET_TARGET_LIBRARIES PARENT_SCOPE)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_library(sushi-driver-socket OBJECT
            winsock/driver.cpp
            winsock/driver.hpp
            )
    list(APPEND DRIVER_SOCKET_TARGET_LIBRARIES
            wsock32
            ws2_32)
    set(SUSHI_DRIVER_SOCKET_NAME "winsock" PARENT_SCOPE)
else()
    add_library(sushi-driver-socket OBJECT
            bsd/driver.cpp
            bsd/driver.hpp
            )
    set(SUSHI_DRIVER_SOCKET_NAME "bsd" PARENT_SCOPE)
endif()