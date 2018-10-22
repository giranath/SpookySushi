#ifndef SPOOKYSUSHI_PHYSIC_SHAPE_HPP
#define SPOOKYSUSHI_PHYSIC_SHAPE_HPP

#include <types.hpp>

namespace sushi {

struct PhysicSphereShape {
    float radius;

    explicit PhysicSphereShape(float radius)
    : radius{radius} {

    }
};

struct PhysicBoxShape {
    float width, height, depth;

    PhysicBoxShape(float width, float height, float depth)
    : width{width}
    , height{height}
    , depth{depth} {

    }
};

struct PhysicCapsuleShape {
    enum Axis {
        AxisX,
        AxisY,
        AxisZ
    };

    float radius;
    float height;
    Axis axis;

    PhysicCapsuleShape(float radius, float height, Axis axis = Axis::AxisY)
    : radius{radius}
    , height{height}
    , axis{axis} {

    }
};

struct PhysicPlaneShape {
    Vec3 normal;
    float distance;

    constexpr PhysicPlaneShape() noexcept
    : normal{0.f, 1.f, 0.f}
    , distance{0.f} {

    }

    constexpr PhysicPlaneShape(const Vec3& normal, float distance = 0.f) noexcept
    : normal{normal}
    , distance{distance} {

    }
};

}

#endif
