#include "physic_movement_lock.hpp"

namespace sushi {

MovementLock::MovementLock()
: lock_flags{0} {

}

bool MovementLock::is_locked(RotationLock lock) const noexcept {
    return (lock_flags & static_cast<uint8_t>(lock)) != 0;
}

bool MovementLock::is_locked(TranslationLock lock) const noexcept {
    return (lock_flags & static_cast<uint8_t>(lock)) != 0;
}

MovementLock& MovementLock::lock(RotationLock lock) noexcept {
    lock_flags |= static_cast<uint8_t>(lock);
    return *this;
}

MovementLock& MovementLock::lock(TranslationLock lock) noexcept {
    lock_flags |= static_cast<uint8_t>(lock);
    return *this;
}

MovementLock& MovementLock::unlock(RotationLock lock) noexcept {
    lock_flags &= ~(static_cast<uint8_t>(lock));

    return *this;
}

MovementLock& MovementLock::unlock(TranslationLock lock) noexcept {
    lock_flags &= ~(static_cast<uint8_t>(lock));

    return *this;
}

MovementLock& MovementLock::lock_all() noexcept {
    lock_flags = 0xFF;

    return *this;
}

MovementLock& MovementLock::unlock_all() noexcept {
    lock_flags = 0;
    return *this;
}

MovementLock::operator uint8_t() const noexcept {
    return lock_flags;
}

}
