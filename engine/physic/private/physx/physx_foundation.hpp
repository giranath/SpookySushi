#ifndef SPOOKYSUSHI_PHYSX_FOUNDATION_HPP
#define SPOOKYSUSHI_PHYSX_FOUNDATION_HPP

namespace physx {

class PxFoundation;

}

namespace sushi {

struct CannotCreatePhysXFoundation{};

class PhysXFoundation {
    physx::PxFoundation* foundation;
public:
    PhysXFoundation();
    ~PhysXFoundation();

    const physx::PxFoundation& get() const noexcept { return *foundation; }
    physx::PxFoundation& get() noexcept { return *foundation; }
};
}

#endif //SPOOKYSUSHI_PHYSX_FOUNDATION_HPP
