#ifndef SPOOKYSUSHI_PHYSIC_JOINTS_HPP
#define SPOOKYSUSHI_PHYSIC_JOINTS_HPP

namespace sushi {

struct PhysicRopeJoint {
    float length;

    explicit PhysicRopeJoint(float length) noexcept
    : length{length} {

    }
};

}

#endif
