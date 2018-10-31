#include "../public/entity.hpp"
#include <limits>

namespace sushi {

const uint32_t Entity::index_mask = 0xFFFFFF00;
const uint32_t Entity::generation_mask = 0x000000FF;
const uint32_t Entity::max_index = 16777215;

Entity::Entity() noexcept
: identifier(0) {

}

Entity::Entity(uint32_t identifier) noexcept
: identifier(identifier) {

}

Entity::Index Entity::index() const noexcept {
    return static_cast<Index>((identifier & index_mask) >> 8);
}

Entity::Generation Entity::generation() const noexcept {
    return static_cast<Generation>(identifier & generation_mask);
}

bool Entity::good() const noexcept {
    return identifier < std::numeric_limits<uint32_t>::max();
}

Entity::operator bool() const noexcept {
    return good();
}


Entity Entity::make(Index index, Generation generation) noexcept {
    return Entity((index << 8) | generation);
}

const Entity Entity::invalid = Entity(std::numeric_limits<uint32_t>::max());

}