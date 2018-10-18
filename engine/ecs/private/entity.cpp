#include "entity.hpp"

#include <stdexcept>

namespace sushi {

entity make_entity(uint32_t index, uint8_t generation) {
    if(index >= 0x00FFFFFF) {
        throw std::out_of_range{"entity index is too big"};
    }

    return (generation << 24) | index;
}

entity_generation generation(entity e) noexcept {
    return static_cast<entity_generation>((e & 0xFF000000) >> 24);
}

entity_index index(entity e) noexcept {
    return e & 0x00FFFFFF;
}

}