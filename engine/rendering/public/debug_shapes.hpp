#ifndef SPOOKYSUSHI_DEBUG_SHAPES_HPP
#define SPOOKYSUSHI_DEBUG_SHAPES_HPP

#include "debug_shape_traits.hpp"

#include <types.hpp>
#include <color.hpp>

#include <vector>

namespace sushi {

struct DebugShape {
    RGBColor color;
    uint32_t cooldown;
    bool enable_depth;

    constexpr DebugShape(RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : color{color}
    , cooldown{cooldown}
    , enable_depth{enable_depth} {

    }

    virtual void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const = 0;

    bool is_dead() const noexcept {
        return cooldown == 0;
    }

    bool operator<(const DebugShape& other) const noexcept {
        // A dead shape if always larger than everything
        if(is_dead()) return false;
        if(other.is_dead()) return true;

        // A depth enabled shape is smaller than a disabled one
        if(enable_depth && !other.enable_depth) {
            return true;
        }

        if(!enable_depth && other.enable_depth) {
            return false;
        }

        // Else the cooldown sort the elements
        return cooldown < other.cooldown;
    }
};

struct DebugCenteredShape : public DebugShape {
    Vec3 center;

    constexpr DebugCenteredShape(Vec3 center, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugShape(color, cooldown, enable_depth)
    , center{center} {

    }
};

static const std::size_t SPHERE_RESOLUTION = 8;
static const std::size_t CIRCLE_RESOLUTION = 16;

struct DebugCircleShape : public DebugCenteredShape {
    Vec3 normal;
    float radius;

    constexpr DebugCircleShape(Vec3 center, float radius, Vec3 normal, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugCenteredShape(center, color, cooldown, enable_depth)
    , normal{normal}
    , radius{radius} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

IMPL_DEBUG_SHAPE_TRAITS(DebugCircleShape, (CIRCLE_RESOLUTION * 2), debug_shape_primitive::Lines);

struct DebugSphereShape : public DebugCenteredShape {
    float radius;

    constexpr DebugSphereShape(Vec3 center, float radius, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugCenteredShape(center, color, cooldown, enable_depth)
    , radius{radius} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

IMPL_DEBUG_SHAPE_TRAITS(DebugSphereShape, (SPHERE_RESOLUTION * SPHERE_RESOLUTION * 6), debug_shape_primitive::Triangles);

struct DebugAABBShape : public DebugCenteredShape {
    Vec3 extend;

    constexpr DebugAABBShape(Vec3 center, Vec3 extend, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugCenteredShape(center, color, cooldown, enable_depth)
    , extend{extend} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

IMPL_DEBUG_SHAPE_TRAITS(DebugAABBShape, 48, debug_shape_primitive::Lines);

struct DebugLineShape : public DebugShape {
    Vec3 start;
    Vec3 end;

    constexpr DebugLineShape(Vec3 start, Vec3 end, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugShape(color, cooldown, enable_depth)
    , start{start}
    , end{end} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

IMPL_DEBUG_SHAPE_TRAITS(DebugLineShape, 2, debug_shape_primitive::Lines);

struct DebugTriangleShape : public DebugShape {
    Vec3 a;
    Vec3 b;
    Vec3 c;

    constexpr DebugTriangleShape(Vec3 a, Vec3 b, Vec3 c, RGBColor color, uint32_t cooldowm = 0, bool enable_depth = true) noexcept
    : DebugShape(color, cooldowm, enable_depth)
    , a{a}
    , b{b}
    , c{c} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

IMPL_DEBUG_SHAPE_TRAITS(DebugTriangleShape, 3, debug_shape_primitive::Triangles);

}

#endif
