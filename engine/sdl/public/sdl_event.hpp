#ifndef SPOOKYSUSHI_EVENT_HPP
#define SPOOKYSUSHI_EVENT_HPP

#include <SDL.h>

#include <iterator>
#include <stdexcept>

namespace sushi {

class MouseRelativeModeNotSupported : public std::runtime_error {
public:
    MouseRelativeModeNotSupported() noexcept
    : std::runtime_error("mouse relative mode not supported") {

    }
};

void set_relative_mouse_mode(bool is_relative);

class PollEventIterator {
    SDL_Event event;
    bool has_next;

    explicit PollEventIterator(bool has_next) noexcept;
public:
    // iterator traits
    using iterator_category = std::input_iterator_tag;
    using value_type = SDL_Event;
    using difference_type = void;
    using pointer = SDL_Event*;
    using const_pointer = const SDL_Event*;
    using reference = SDL_Event&;
    using const_reference = const SDL_Event&;

    PollEventIterator();

    PollEventIterator& operator++() noexcept;
    PollEventIterator operator++(int) noexcept;
    bool operator==(const PollEventIterator& other) const noexcept;
    bool operator!=(const PollEventIterator& other) const noexcept;

    pointer operator->() noexcept;
    const_pointer operator->() const noexcept;

    reference operator*() noexcept;
    const_reference operator*() const noexcept;

    friend PollEventIterator begin(PollEventIterator) noexcept;
    friend PollEventIterator end(const PollEventIterator&) noexcept;
};

}


#endif //SPOOKYSUSHI_EVENT_HPP
