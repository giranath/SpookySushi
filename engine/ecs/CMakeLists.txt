add_library(sushi-ecs OBJECT
        entity.hpp
        storage_traits.hpp
        tree_storage.hpp
        indirect_vector_storage.hpp
        entity_iterator.hpp
        null_storage.cpp
        null_storage.hpp
        entity_tree.cpp
        entity_tree.hpp
        )