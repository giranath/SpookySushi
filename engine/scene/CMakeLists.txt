add_library(sushi-scene
        public/scene_graph.hpp
        private/scene_graph.cpp
        public/scene_graph_pool.hpp)

target_link_libraries(sushi-scene
        PUBLIC sushi-core
        PUBLIC sushi-ecs
        PUBLIC sushi-async)
target_include_directories(sushi-scene
        PUBLIC public
        PRIVATE private)