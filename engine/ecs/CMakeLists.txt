add_library(sushi-ecs
        private/entity.cpp
        public/entity.hpp
        private/entity_registry.cpp
        public/entity_registry.hpp)
target_include_directories(sushi-ecs
        PUBLIC public
        PRIVATE private)