#ifndef SPOOKYSUSHI_ENTITY_HPP
#define SPOOKYSUSHI_ENTITY_HPP

#include <cstdint>

namespace sushi {

class Entity {
public:
    using Generation = uint8_t;
    using Index = uint32_t;

private:
    static const uint32_t index_mask;
    static const uint32_t generation_mask;
public:
    static const uint32_t max_index;
private:
    uint32_t identifier;

public:
    Entity() noexcept;
    explicit Entity(uint32_t identifier) noexcept;

    Index index() const noexcept;
    Generation generation() const noexcept;
    uint32_t value() const noexcept;

    bool good() const noexcept;
    explicit operator bool() const noexcept;
    explicit operator uint32_t() const noexcept;

    static Entity make(Index index, Generation generation) noexcept;
    static const Entity invalid;
};



}

#endif //SPOOKYSUSHI_ENTITY_HPP
