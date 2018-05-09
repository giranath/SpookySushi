#ifndef SPOOKYSUSHI_ENTITY_HPP
#define SPOOKYSUSHI_ENTITY_HPP

#include <cstdint>
#include <limits>

namespace sushi { namespace ecs {

using entity = uint32_t;
constexpr const entity ROOT_ENTITY = std::numeric_limits<entity>::min();
constexpr const entity INVALID_ENTITY = ROOT_ENTITY;
}}

#endif
