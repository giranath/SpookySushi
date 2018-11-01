add_library(sushi-debug
        private/profiler.cpp
        public/profiler.hpp
        private/logger.cpp
        public/logger.hpp)
target_include_directories(sushi-debug
        PUBLIC public
        PRIVATE private)
target_link_libraries(sushi-debug
        PUBLIC sushi-async
        PRIVATE sushi-drivers)
if(ENABLE_PROFILING)
    target_compile_definitions(sushi-debug
            PUBLIC SUSHI_ENABLE_PROFILING)
endif()