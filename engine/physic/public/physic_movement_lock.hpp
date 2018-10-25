#ifndef SPOOKYSUSHI_PHYSIC_MOVEMENT_LOCK_HPP
#define SPOOKYSUSHI_PHYSIC_MOVEMENT_LOCK_HPP

#include <cstdint>

namespace sushi {

enum class RotationLock : uint8_t {
    Head = 1,
    Roll = (1 << 1),
    Tilt = (1 << 2)
};

enum class TranslationLock : uint8_t {
    XAxis = (1 << 3),
    YAxis = (1 << 4),
    ZAxis = (1 << 5)
};

class MovementLock {
    uint8_t lock_flags;

public:
    MovementLock();

    bool is_locked(RotationLock lock) const noexcept;
    bool is_locked(TranslationLock lock) const noexcept;

    MovementLock& lock(RotationLock lock) noexcept;
    MovementLock& lock(TranslationLock lock) noexcept;

    MovementLock& unlock(RotationLock lock) noexcept;
    MovementLock& unlock(TranslationLock lock) noexcept;

    MovementLock& lock_all() noexcept;
    MovementLock& unlock_all() noexcept;

    explicit operator uint8_t() const noexcept;
};

}

#endif
