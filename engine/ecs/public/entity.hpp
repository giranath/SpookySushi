#ifndef SPOOKYSUSHI_ENTITY_HPP
#define SPOOKYSUSHI_ENTITY_HPP

#include <cstdint>

namespace sushi {

struct Entity {
    using Generation = uint8_t;
    using Index = uint32_t;

    static const uint32_t index_mask;
    static const uint32_t generation_mask;
    static const uint32_t max_index;

    uint32_t identifier;

    Entity() noexcept;
    explicit Entity(uint32_t identifier) noexcept;

    Index index() const noexcept;
    Generation generation() const noexcept;

    bool good() const noexcept;
    explicit operator bool() const noexcept;

    static Entity make(Index index, Generation generation) noexcept;
    static const Entity invalid;
};



}

#endif //SPOOKYSUSHI_ENTITY_HPP
