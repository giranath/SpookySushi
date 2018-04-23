#include "event.hpp"

namespace sushi {

poll_event_iterator::poll_event_iterator(bool has_next) noexcept
: event()
, has_next{has_next} {

}

poll_event_iterator::poll_event_iterator()
: event()
, has_next() {
    has_next = SDL_PollEvent(&event) == 1;
}

poll_event_iterator& poll_event_iterator::operator++() noexcept {
    has_next = SDL_PollEvent(&event) == 1;

    return *this;
}

poll_event_iterator poll_event_iterator::operator++(int) noexcept {
    poll_event_iterator old = *this;

    ++(*this);

    return old;
}

bool poll_event_iterator::operator==(const poll_event_iterator& other) const noexcept {
    return has_next == other.has_next;
}

bool poll_event_iterator::operator!=(const poll_event_iterator& other) const noexcept {
    return has_next != other.has_next;
}

SDL_Event* poll_event_iterator::operator->() noexcept {
    return &event;
}

const SDL_Event* poll_event_iterator::operator->() const noexcept {
    return &event;
}

SDL_Event& poll_event_iterator::operator*() noexcept {
    return event;
}

const SDL_Event& poll_event_iterator::operator*() const noexcept {
    return event;
}

poll_event_iterator begin(poll_event_iterator it) noexcept {
    return it;
}

poll_event_iterator end(const poll_event_iterator&) noexcept {
    return poll_event_iterator(false);
}

std::optional<SDL_Event> wait_event() noexcept {
    SDL_Event ev;

    if(SDL_WaitEvent(&ev)) {
        return std::optional<SDL_Event>(ev);
    }

    return {};
}

}