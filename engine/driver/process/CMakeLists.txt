set(SUSHI_HAS_PROCESS_SUPPORT ON PARENT_SCOPE)
set(DRIVER_PROCESS_TARGET_LIBRARIES PARENT_SCOPE)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_library(sushi-driver-process OBJECT
            process.hpp
            win32/driver.cpp
            )
    set(SUSHI_DRIVER_PROCESS_NAME "windows" PARENT_SCOPE)
else()
    add_library(sushi-driver-process OBJECT
            process.hpp
            posix/driver.cpp
            )
    set(SUSHI_DRIVER_PROCESS_NAME "posix" PARENT_SCOPE)
endif()