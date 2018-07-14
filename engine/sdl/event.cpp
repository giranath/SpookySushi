#include "event.hpp"

namespace sushi {

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

SDL_Event* PollEventIterator::operator->() noexcept {
    return &event;
}

const SDL_Event* PollEventIterator::operator->() const noexcept {
    return &event;
}

SDL_Event& PollEventIterator::operator*() noexcept {
    return event;
}

const SDL_Event& PollEventIterator::operator*() const noexcept {
    return event;
}

PollEventIterator begin(PollEventIterator it) noexcept {
    return it;
}

PollEventIterator end(const PollEventIterator&) noexcept {
    return PollEventIterator(false);
}

}