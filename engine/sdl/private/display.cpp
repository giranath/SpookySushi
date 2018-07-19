#include "display.hpp"
#include <SDL.h>

namespace sushi {

Display::Display(int index) noexcept
: index{index} {

}

recti Display::bounds() const noexcept {
    SDL_Rect r;
    if(SDL_GetDisplayBounds(index, &r) == 0) {
        return recti::make_with_dimension(r.x, r.y, r.w, r.h);
    }

    return recti{};
}

recti Display::usable_bounds() const noexcept {
    SDL_Rect r;
    if(SDL_GetDisplayUsableBounds(index, &r) == 0) {
        return recti::make_with_dimension(r.x, r.y, r.w, r.h);
    }

    return recti{};
}

const char* Display::name() const noexcept {
    return SDL_GetDisplayName(index);
}

bool Display::try_get_dpi(DpiInfos& infos) const noexcept {
    return SDL_GetDisplayDPI(index, &infos.diagonal , &infos.horizontal, &infos.vertical) == 0;
}

DisplayIterator::DisplayIterator() noexcept
: d{SDL_GetNumVideoDisplays()}{

}

DisplayIterator::DisplayIterator(int index) noexcept
: d{index} {

}

DisplayIterator& DisplayIterator::operator++() {
    ++d.index;

    return *this;
}

DisplayIterator DisplayIterator::operator++(int) {
    DisplayIterator temp = *this;

    ++(*this);

    return temp;
}

bool DisplayIterator::operator==(const DisplayIterator &other) const noexcept {
    return d.index == other.d.index;
}

bool DisplayIterator::operator!=(const DisplayIterator &other) const noexcept {
    return d.index != other.d.index;
}

DisplayIterator::pointer DisplayIterator::operator->() const noexcept {
    return &d;
}

DisplayIterator::reference DisplayIterator::operator*() const noexcept {
    return d;
}

DisplayIterator begin(DisplayIterator it) noexcept {
    return DisplayIterator{0};
}

DisplayIterator end(DisplayIterator& it) noexcept {
    return DisplayIterator{};
}

}