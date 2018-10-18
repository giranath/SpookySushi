add_library(sushi-ecs
        public/entity.hpp
        private/entity.cpp
        public/component_storage_traits.hpp
        public/dictionary_component_storage.hpp
        public/indirect_contiguous_component_storage.hpp
        )
target_include_directories(sushi-ecs
        PUBLIC public
        PRIVATE private)