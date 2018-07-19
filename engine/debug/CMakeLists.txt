add_library(sushi-debug
        private/profiler.cpp
        public/profiler.hpp
        private/logger.cpp
        public/logger.hpp
        private/request.cpp
        public/request.hpp
        private/response.cpp
        public/response.hpp
        private/server.cpp
        public/server.hpp)
target_include_directories(sushi-debug
        PUBLIC public
        PRIVATE private)
target_link_libraries(sushi-debug
        PUBLIC sushi-async)