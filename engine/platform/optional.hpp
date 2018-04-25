#ifndef SPOOKYSUSHI_OPTIONAL_HPP
#define SPOOKYSUSHI_OPTIONAL_HPP

#include <new>
#include <cassert>

#include <experimental/optional>
namespace sushi {

class bad_optional_access {};

template<typename Type>
class optional {
public:
    using value_type = Type;
private:
    union {
        char null_state;
        value_type value_storage;
    };
    bool is_present;
public:
    ~optional() noexcept {
        if(is_present) {
            value_storage.~value_type();
        }
    }

    constexpr optional() noexcept
    : null_state(0)
    , is_present{false} {

    }

    constexpr optional(const value_type& value)
    : is_present{true} {
        if(is_present) {
            new(reinterpret_cast<char*>(&value_storage)) value_type{value};
        }
    }

    constexpr optional(value_type&& value) noexcept
    : is_present{true} {
        if(is_present) {
            new(reinterpret_cast<char*>(&value_storage)) value_type{std::move(value)};
        }
    }

    optional& operator=(const optional& other) {
        if(is_present && other.is_present) {
            if (is_present) {
                value_storage = other.value_storage;
            }
        }
        else {
            if(is_present) {
                value_storage->~value_type();
            }
            else {
                new(reinterpret_cast<char*>(&value_storage)) value_type(other.value_storage);
            }

            is_present = other.is_present;
        }

        return *this;
    }

    optional& operator=(optional&& other) noexcept {
        if(is_present && other.is_present) {
            if (is_present) {
                value_storage = std::move(other.value_storage);
            }
        }
        else {
            if(is_present) {
                value_storage.~value_type();
            }
            else {
                new(reinterpret_cast<char*>(&value_storage)) value_type(std::move(other.value_storage));
            }

            is_present = other.is_present;
        }

        return *this;
    }

    void swap(optional& other) noexcept {
        using std::swap;

        if(is_present && other.is_present) {
            if(is_present) {
                swap(value_storage, other.value_storage);
            }
        }
        else {
            if(is_present) {
                new(reinterpret_cast<char*>(&other.value_storage)) value_type{value_storage};
                value_storage.~value_type();
            }
            else {
                new(reinterpret_cast<char*>(&value_storage)) value_type{other.value_storage};
                other.value_storage.~value_type();
            }

            swap(is_present, other.is_present);
        }
    }

    constexpr explicit operator bool() const noexcept {
        return is_present;
    }

    constexpr value_type* operator->() noexcept {
        return &value_storage;
    }

    constexpr const value_type* operator->() const noexcept {
        return &value_storage;
    }

    constexpr value_type& operator*() noexcept {
        return value_storage;
    }

    constexpr const value_type& operator*() const noexcept {
        return value_storage;
    }
};

}

#endif //SPOOKYSUSHI_OPTIONAL_HPP
