add_library(sushi-service INTERFACE)
target_link_libraries(sushi-service
        INTERFACE sushi-core
        INTERFACE sushi-debug
        INTERFACE sushi-input
        INTERFACE sushi-rendering
        INTERFACE sushi-async)
target_include_directories(sushi-service
        INTERFACE public)