#include "sdl_event.hpp"

namespace sushi {

void set_relative_mouse_mode(bool is_relative) {
    int result = SDL_SetRelativeMouseMode(is_relative ? SDL_TRUE : SDL_FALSE);

    if(result == -1) {
        throw MouseRelativeModeNotSupported{};
    }
}

PollEventIterator::PollEventIterator(bool has_next) noexcept
: event()
, has_next{has_next} {

}

PollEventIterator::PollEventIterator()
: event()
, has_next() {
    has_next = SDL_PollEvent(&event) == 1;
}

PollEventIterator& PollEventIterator::operator++() noexcept {
    has_next = SDL_PollEvent(&event) == 1;

    return *this;
}

PollEventIterator PollEventIterator::operator++(int) noexcept {
    PollEventIterator old = *this;

    ++(*this);

    return old;
}

bool PollEventIterator::operator==(const PollEventIterator& other) const noexcept {
    return has_next == other.has_next;
}

bool PollEventIterator::operator!=(const PollEventIterator& other) const noexcept {
    return has_next != other.has_next;
}

PollEventIterator::pointer PollEventIterator::operator->() noexcept {
    return &event;
}

PollEventIterator::const_pointer PollEventIterator::operator->() const noexcept {
    return &event;
}

PollEventIterator::reference PollEventIterator::operator*() noexcept {
    return event;
}

PollEventIterator::const_reference PollEventIterator::operator*() const noexcept {
    return event;
}

PollEventIterator begin(PollEventIterator it) noexcept {
    return it;
}

PollEventIterator end(const PollEventIterator&) noexcept {
    return PollEventIterator(false);
}

}