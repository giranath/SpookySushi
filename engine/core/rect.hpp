#ifndef SPOOKYSUSHI_RECT_HPP
#define SPOOKYSUSHI_RECT_HPP

#include <type_traits>

namespace sushi {

template<typename T>
struct rect {
    static_assert(std::is_arithmetic<T>::value, "you must specify an arithmetic type");

    const T left;
    const T right;
    const T bottom;
    const T top;

    constexpr rect() noexcept
    : left{}
    , right{}
    , bottom{}
    , top{} {

    }

    constexpr rect(T width, T height) noexcept
    : left{}
    , right{width}
    , bottom{}
    , top{height} {

    }

    constexpr rect(T left, T right, T bottom, T top) noexcept
    : left{left}
    , right{right}
    , bottom{bottom}
    , top{top} {

    }

    constexpr T width() const noexcept {
        return right - left;
    }

    constexpr T height() const noexcept {
        return top - bottom;
    }

    static constexpr rect make_with_dimension(T left, T bottom, T width, T height) noexcept {
        return rect{left, left + width, bottom, bottom + height};
    }
};

using rectf = rect<float>;
using rectd = rect<double>;
using recti = rect<int>;

}

#endif //SPOOKYSUSHI_RECT_HPP
