#ifndef SPOOKYSUSHI_COLOR_HPP
#define SPOOKYSUSHI_COLOR_HPP

#include <cstdint>

namespace sushi {

enum class Colors {
    White,
    Black,
    Transparent,
    Red,
    Green,
    Blue,
    Cyan,
    Magenta,
    Orange,
    Yellow,
    Brown,
    Gray,
    DarkGray,
    LigthGray
};

struct RGBColor {
    using component_type = uint8_t;

    component_type r;
    component_type g;
    component_type b;

    constexpr RGBColor() noexcept
    : r{}
    , g{}
    , b{} {

    }

    constexpr RGBColor(Colors color) noexcept
    : RGBColor() {
        switch(color) {
            case Colors::Black:
                r = 0;
                g = 0;
                b = 0;
                break;
            case Colors::Blue:
                r = 0;
                g = 0;
                b = 255;
                break;
            case Colors::Brown:
                r = 0;
                g = 0;
                b = 255;
                break;
            case Colors::Cyan:
                r = 0;
                g = 255;
                b = 255;
                break;
            case Colors::DarkGray:
                r = 55;
                g = 55;
                b = 55;
                break;
            case Colors::Gray:
                r = 128;
                g = 128;
                b = 128;
                break;
            case Colors::Green:
                r = 0;
                g = 255;
                b = 0;
                break;
            case Colors::LigthGray:
                r = 200;
                g = 200;
                b = 200;
                break;
            case Colors::Magenta:
                r = 255;
                g = 0;
                b = 255;
                break;
            case Colors::Orange:
                r = 255;
                g = 165;
                b = 0;
                break;
            case Colors::Red:
                r = 255;
                g = 0;
                b = 0;
                break;
            case Colors::Transparent:
                r = 255;
                g = 255;
                b = 255;
                break;
            case Colors::White:
                r = 255;
                g = 255;
                b = 255;
                break;
            case Colors::Yellow:
                r = 255;
                g = 255;
                b = 0;
                break;
        }
    }

    constexpr RGBColor(component_type r, component_type g, component_type b) noexcept
    : r{r}
    , g{g}
    , b{b} {

    }

    constexpr float gray() const noexcept {
        return 0.299f * r + 0.587f * g + 0.114f * b;
    }

    constexpr bool operator==(const RGBColor& other) const noexcept {
        return r == other.r && g == other.g && b == other.b;
    }

    constexpr bool operator!=(const RGBColor& other) const noexcept {
        return r != other.r || g != other.g || b != other.b;
    }

    constexpr RGBColor operator+(const RGBColor& other) const noexcept {
        return RGBColor(r + other.r, g + other.g, b + other.b);
    }

    constexpr RGBColor operator-(const RGBColor& other) const noexcept {
        return RGBColor(r - other.r, g - other.g, b - other.b);
    }
};

struct RGBAColor {
    using component_type = uint8_t;

    component_type r;
    component_type g;
    component_type b;
    component_type a;

    constexpr RGBAColor() noexcept
    : r{}
    , g{}
    , b{}
    , a{255} {

    }

    constexpr RGBAColor(component_type r, component_type g, component_type b, component_type a = 255) noexcept
    : r{r}
    , g{g}
    , b{b}
    , a{a} {

    }

    constexpr RGBAColor(Colors color)
    : RGBAColor{} {
        const RGBColor c(color);
        r = c.r;
        g = c.g;
        b = c.b;

        if(color == Colors::Transparent) {
            a = 0;
        }
    }

    constexpr float gray() const noexcept {
        return 0.299f * r + 0.587f * g + 0.114f * b;
    }

    constexpr bool operator==(const RGBAColor& other) const noexcept {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    constexpr bool operator!=(const RGBAColor& other) const noexcept {
        return r != other.r || g != other.g || b != other.b || a != other.a;
    }

    constexpr RGBAColor operator+(const RGBAColor& other) const noexcept {
        return RGBAColor(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    constexpr RGBAColor operator-(const RGBAColor& other) const noexcept {
        return RGBAColor(r - other.r, g - other.g, b - other.b, a - other.a);
    }
};

}

#endif
