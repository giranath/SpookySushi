set(SUSHI_HAS_DYNAMIC_LINKING_SUPPORT ON PARENT_SCOPE)
set(DRIVER_DYNAMIC_LINKING_TARGET_LIBRARIES PARENT_SCOPE)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_library(sushi-driver-dynamic-linking OBJECT
            windows/driver.cpp
            dynamic_linking.hpp
            )
    set(SUSHI_DRIVER_DYNAMIC_LINKING_NAME "windows" PARENT_SCOPE)
else()
    add_library(sushi-driver-dynamic-linking OBJECT
            posix/driver.cpp
            dynamic_linking.hpp
            )
    set(SUSHI_DRIVER_DYNAMIC_LINKING_NAME "posix" PARENT_SCOPE)
endif()

list(APPEND DRIVER_DYNAMIC_LINKING_TARGET_LIBRARIES ${CMAKE_DL_LIBS})