add_library(sushi-async
        private/job.cpp
        public/job.hpp
        private/worker.cpp
        public/worker.hpp
        private/engine.cpp
        public/engine.hpp
        private/job_pool.cpp
        public/job_pool.hpp
        public/scmp_queue.hpp
        public/event.hpp
        )
target_include_directories(sushi-async
        PUBLIC public
        PRIVATE private)