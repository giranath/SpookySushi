add_library(sushi-ecs
        private/entity.cpp
        public/entity.hpp
        private/entity_registry.cpp
        public/entity_registry.hpp public/component_storage.hpp public/map_storage.hpp public/vector_storage.hpp)
target_include_directories(sushi-ecs
        PUBLIC public
        PRIVATE private)

add_executable(sushi-ecs-tests
        test/main.cpp)
target_link_libraries(sushi-ecs-tests
        PUBLIC sushi-ecs
        PUBLIC libCatch2)

add_test(NAME ecs
        COMMAND sushi-ecs-tests)