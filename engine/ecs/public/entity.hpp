#ifndef SPOOKYSUSHI_ENTITY_HPP
#define SPOOKYSUSHI_ENTITY_HPP

#include <cstdint>

namespace sushi {

struct Entity {
    using Generation = uint8_t;
    using Index = uint32_t;

    static const uint64_t IndexBits;
    static const uint64_t IndexMask;

    static const uint64_t GenerationBits;
    static const uint64_t GenerationMask;
    uint64_t identifier;

    Entity() noexcept;
    explicit Entity(uint64_t identifier) noexcept;

    Index index() const noexcept;
    Generation generation() const noexcept;

    static Entity make(Index index, Generation generation) noexcept;
};

}

#endif //SPOOKYSUSHI_ENTITY_HPP
