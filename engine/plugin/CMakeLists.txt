add_library(sushi-plugin
        public/runtime_plugin.hpp
        private/shared_library_runtime_plugin.cpp
        private/shared_library_runtime_plugin.hpp public/plugin_error_codes.h)

target_link_libraries(sushi-plugin
        PUBLIC sushi-core
        PUBLIC sushi-service
        PRIVATE ${CMAKE_DL_LIBS})
target_include_directories(sushi-plugin
        PUBLIC public
        PRIVATE private)