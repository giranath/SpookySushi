#include "../public/entity_registry.hpp"
#include <cassert>

namespace sushi {

const std::size_t EntityRegistry::MinimumFreeIndiceCount = 1024;

Entity EntityRegistry::create() {
    Entity::Index index;

    if(free_indices.size() >= MinimumFreeIndiceCount) {
        index = free_indices.front();
        free_indices.pop_front();
    }
    else {
        generations.push_back(0);
        index = static_cast<uint32_t>(generations.size() - 1);
        assert(index < Entity::max_index);
    }

    return Entity::make(index, generations[index]);
}

bool EntityRegistry::is_alive(const Entity& e) const noexcept {
    assert(e.index() < generations.size());

    return generations[e.index()] == e.generation();
}

void EntityRegistry::destroy(const Entity& e) {
    const Entity::Index index = e.index();

    // Make sure we're removing an entity that is alive
    assert(index < generations.size());
    assert(generations[index] == e.generation());

    ++generations[index];
    free_indices.push_back(index);
}

}