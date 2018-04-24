#ifndef SPOOKYSUSHI_EVENT_HPP
#define SPOOKYSUSHI_EVENT_HPP

#include <SDL.h>

#include <iterator>
#include <optional>

namespace sushi {

class poll_event_iterator : public std::iterator<std::input_iterator_tag, SDL_Event, SDL_Event, SDL_Event*, SDL_Event&>{
    SDL_Event event;
    bool has_next;

    poll_event_iterator(bool has_next) noexcept;
public:
    poll_event_iterator();

    poll_event_iterator& operator++() noexcept;
    poll_event_iterator operator++(int) noexcept;
    bool operator==(const poll_event_iterator& other) const noexcept;
    bool operator!=(const poll_event_iterator& other) const noexcept;

    SDL_Event* operator->() noexcept;
    const SDL_Event* operator->() const noexcept;

    SDL_Event& operator*() noexcept;
    const SDL_Event& operator*() const noexcept;

    friend poll_event_iterator begin(poll_event_iterator) noexcept;
    friend poll_event_iterator end(const poll_event_iterator&) noexcept;
};

std::optional<SDL_Event> wait_event() noexcept;

}


#endif //SPOOKYSUSHI_EVENT_HPP
