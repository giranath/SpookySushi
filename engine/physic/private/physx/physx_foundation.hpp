#ifndef SPOOKYSUSHI_PHYSX_FOUNDATION_HPP
#define SPOOKYSUSHI_PHYSX_FOUNDATION_HPP

#include "physx_physics.h"

namespace physx {

class PxFoundation;

}

namespace sushi {

struct CannotCreatePhysXFoundation{};
struct CannotCreatePhysXPhysics{};

class PhysXFoundation {
    physx::PxFoundation* foundation;
public:
    PhysXFoundation();
    ~PhysXFoundation();

    const physx::PxFoundation& get() const noexcept { return *foundation; }
    physx::PxFoundation& get() noexcept { return *foundation; }

    PhysXPhysics make_physics() const;
};
}

#endif //SPOOKYSUSHI_PHYSX_FOUNDATION_HPP
