#ifndef SPOOKYSUSHI_TYPES_HPP
#define SPOOKYSUSHI_TYPES_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstdint>
#include <type_traits>

namespace sushi {

using FloatPrecision = float;
static_assert(std::is_floating_point<FloatPrecision>::value, "float_precision must be a floating point type");

using Scalar = FloatPrecision;

// Vector types
using Vec2 = glm::tvec2<FloatPrecision>;
using Vec3 = glm::tvec3<FloatPrecision>;
using Vec4 = glm::tvec4<FloatPrecision>;

// Matrice types
using Mat2x2 = glm::tmat2x2<FloatPrecision>;
using Mat3x3 = glm::tmat3x3<FloatPrecision>;
using Mat4x4 = glm::tmat4x4<FloatPrecision>;

// Quaternion types
using Quaternion = glm::tquat<FloatPrecision>;

}

#endif
