#ifndef SPOOKYSUSHI_ENTITY_HPP
#define SPOOKYSUSHI_ENTITY_HPP

#include <cstdint>

namespace sushi {

using entity_index = uint32_t;
using entity_generation = uint8_t;

using entity = uint32_t;

entity make_entity(uint32_t index, uint8_t generation);
entity_generation generation(entity e) noexcept;
entity_index index(entity e) noexcept;

}

#endif
