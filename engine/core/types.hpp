#ifndef SPOOKYSUSHI_TYPES_HPP
#define SPOOKYSUSHI_TYPES_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstdint>
#include <type_traits>

namespace sushi {

using float_precision = float;
static_assert(std::is_floating_point<float_precision>::value, "float_precision must be a floating point type");

using scalar = float_precision;

// Vector types
using vec2 = glm::tvec2<float_precision>;
using vec3 = glm::tvec3<float_precision>;
using vec4 = glm::tvec4<float_precision>;

// Matrice types
using mat2 = glm::tmat2x2<float_precision>;
using mat3 = glm::tmat3x3<float_precision>;
using mat4 = glm::tmat4x4<float_precision>;

// Quaternion types
using quaternion = glm::tquat<float_precision>;

}

#endif
