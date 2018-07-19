#include "../public/entity.hpp"

namespace sushi {

const uint64_t Entity::IndexBits = 22;
const uint64_t Entity::IndexMask = static_cast<uint64_t>((1 << IndexBits) - 1);

const uint64_t Entity::GenerationBits = 8;
const uint64_t Entity::GenerationMask = static_cast<uint64_t>((1 << GenerationBits) - 1);

Entity::Entity() noexcept
: identifier(0) {

}

Entity::Entity(uint64_t identifier) noexcept
: identifier(identifier) {

}

Entity::Index Entity::index() const noexcept {
    return static_cast<Index>((identifier >> IndexBits) & IndexMask);
}

Entity::Generation Entity::generation() const noexcept {
    return static_cast<Generation>((identifier >> GenerationBits) & GenerationMask);
}

Entity Entity::make(Index index, Generation generation) noexcept {
    return Entity();
}

}