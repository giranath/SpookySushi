if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SUSHI_SUPPORT_THREAD_AFFINITY 1)
else()
    set(SUSHI_SUPPORT_THREAD_AFFINITY 0)
endif()