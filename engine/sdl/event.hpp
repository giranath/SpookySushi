#ifndef SPOOKYSUSHI_EVENT_HPP
#define SPOOKYSUSHI_EVENT_HPP

#include "../core/optional.hpp"

#include <SDL.h>

#include <iterator>

namespace sushi {

class PollEventIterator : public std::iterator<std::input_iterator_tag, SDL_Event, SDL_Event, SDL_Event*, SDL_Event&>{
    SDL_Event event;
    bool has_next;

    explicit PollEventIterator(bool has_next) noexcept;
public:
    PollEventIterator();

    PollEventIterator& operator++() noexcept;
    PollEventIterator operator++(int) noexcept;
    bool operator==(const PollEventIterator& other) const noexcept;
    bool operator!=(const PollEventIterator& other) const noexcept;

    SDL_Event* operator->() noexcept;
    const SDL_Event* operator->() const noexcept;

    SDL_Event& operator*() noexcept;
    const SDL_Event& operator*() const noexcept;

    friend PollEventIterator begin(PollEventIterator) noexcept;
    friend PollEventIterator end(const PollEventIterator&) noexcept;
};

Optional<SDL_Event> wait_event() noexcept;

}


#endif //SPOOKYSUSHI_EVENT_HPP
