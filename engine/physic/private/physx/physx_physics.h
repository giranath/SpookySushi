#ifndef SPOOKYSUSHI_PHYSX_PHYSICS_H
#define SPOOKYSUSHI_PHYSX_PHYSICS_H

namespace physx {

class PxPhysics;
class PxScene;

}

namespace sushi {

class PhysXFoundation;

class PhysXPhysics {
    friend PhysXFoundation;
    physx::PxPhysics* physics;

    explicit PhysXPhysics(physx::PxPhysics* physics) noexcept;
public:
    PhysXPhysics(const PhysXPhysics&) = delete;
    PhysXPhysics& operator=(const PhysXPhysics&) = delete;

    ~PhysXPhysics() noexcept;

    physx::PxPhysics& get() noexcept { return *physics; }
    const physx::PxPhysics& get() const noexcept { return *physics; }

    // TODO: Wrap PxScene
    physx::PxScene* make_scene();
};
}


#endif //SPOOKYSUSHI_PHYSX_PHYSICS_H
