if(ENABLE_EDITOR)
    add_library(sushi-editor
            public/editor_endpoint.hpp
            private/editor_endpoint.cpp
            public/editor_headers.hpp
            private/editor_headers.cpp)
    target_include_directories(sushi-editor
            PUBLIC public
            PRIVATE private)
    target_link_libraries(sushi-editor
            PUBLIC sushi-core)
    target_compile_definitions(sushi-editor
            PUBLIC SUSHI_EDITOR)
else()
    add_library(sushi-editor INTERFACE)
endif()