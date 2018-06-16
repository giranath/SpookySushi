#include "display.hpp"
#include <SDL.h>

namespace sushi {

display::display(int index) noexcept
: index{index} {

}

recti display::bounds() const noexcept {
    SDL_Rect r;
    if(SDL_GetDisplayBounds(index, &r) == 0) {
        return recti::make_with_dimension(r.x, r.y, r.w, r.h);
    }

    return recti{};
}

recti display::usable_bounds() const noexcept {
    SDL_Rect r;
    if(SDL_GetDisplayUsableBounds(index, &r) == 0) {
        return recti::make_with_dimension(r.x, r.y, r.w, r.h);
    }

    return recti{};
}

const char* display::name() const noexcept {
    return SDL_GetDisplayName(index);
}

Optional<display::dpi_infos> display::dpi() const noexcept {
    float diagonal, horizontal, vertical;

    if(SDL_GetDisplayDPI(index, &diagonal, &horizontal, &vertical) == 0) {
        return { dpi_infos{diagonal, horizontal, vertical} };
    }

    return {};
}

display_iterator::display_iterator() noexcept
: d{SDL_GetNumVideoDisplays()}{

}

display_iterator::display_iterator(int index) noexcept
: d{index} {

}

display_iterator& display_iterator::operator++() {
    ++d.index;

    return *this;
}

display_iterator display_iterator::operator++(int) {
    display_iterator temp = *this;

    ++(*this);

    return temp;
}

bool display_iterator::operator==(const display_iterator &other) const noexcept {
    return d.index == other.d.index;
}

bool display_iterator::operator!=(const display_iterator &other) const noexcept {
    return d.index != other.d.index;
}

display_iterator::pointer display_iterator::operator->() const noexcept {
    return &d;
}

display_iterator::reference display_iterator::operator*() const noexcept {
    return d;
}

display_iterator begin(display_iterator it) noexcept {
    return display_iterator{0};
}

display_iterator end(display_iterator& it) noexcept {
    return display_iterator{};
}

}