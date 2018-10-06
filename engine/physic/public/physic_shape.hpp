#ifndef SPOOKYSUSHI_PHYSIC_SHAPE_HPP
#define SPOOKYSUSHI_PHYSIC_SHAPE_HPP

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

}

#endif
