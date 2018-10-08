#ifndef SPOOKYSUSHI_PHYSICTRANSFORM_HPP
#define SPOOKYSUSHI_PHYSICTRANSFORM_HPP

#include <types.hpp>

namespace sushi {

struct PhysicTransform {
    Vec3 translation;
    Quaternion rotation;

    PhysicTransform() = default;

    explicit PhysicTransform(Vec3 translation) noexcept
    : translation{translation}
    , rotation{} {

    }

    explicit PhysicTransform(Quaternion rotation) noexcept
    : translation{}
    , rotation{rotation} {

    }

    PhysicTransform(Vec3 translation, Quaternion rotation) noexcept
    : translation{translation}
    , rotation{rotation} {

    }
};

}

#endif