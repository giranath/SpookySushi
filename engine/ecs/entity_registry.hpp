#ifndef SPOOKYSUSHI_ENTITY_REGISTRY_HPP
#define SPOOKYSUSHI_ENTITY_REGISTRY_HPP

#include "entity.hpp"

#include <vector>
#include <deque>
#include <cstdint>
#include <iterator>

namespace sushi {

class EntityRegistry {
    static const std::size_t MinimumFreeIndiceCount;
    std::vector<Entity::Generation> generations;
    std::deque<Entity::Index> free_indices;
public:
    // Creates a single entity
    Entity create();

    // Creates multiple entities
    template<typename OutEntityIterator>
    OutEntityIterator create(std::size_t count, OutEntityIterator out) {
        static_assert(std::is_same<typename std::iterator_traits<OutEntityIterator>::value_type, Entity>::value, "iterators must reference Entities");

        for(std::size_t i = 0; i < count; ++i) {
            *out = create();
            ++out;
        }

        return out;
    }

    // Check if an entity is alive
    bool is_alive(const Entity& e) const noexcept;

    // Destroys a single entity
    void destroy(const Entity& e);

    // Destroys multiple entities
    template<typename EntityIterator>
    void destroy(EntityIterator begin, EntityIterator end) {
        static_assert(std::is_same<typename std::iterator_traits<EntityIterator>::value_type, Entity>::value, "iterators must reference Entities");

        while(begin != end) {
            destroy(*begin);
            ++begin;
        }
    }
};

}

#endif
