#ifndef SPOOKYSUSHI_DISPLAY_HPP
#define SPOOKYSUSHI_DISPLAY_HPP

#include "../core/rect.hpp"
#include "../core/optional.hpp"

#include <iterator>

namespace sushi {

class DisplayIterator;

class Display {
    friend DisplayIterator;
    int index;

public:
    struct DpiInfos {
        float diagonal;
        float horizontal;
        float vertical;

        DpiInfos(float d, float h, float v)
        : diagonal{d}, horizontal{h}, vertical{v} {

        }
    };

    explicit Display(int index) noexcept;

    recti bounds() const noexcept;
    recti usable_bounds() const noexcept;
    const char* name() const noexcept;

    Optional<DpiInfos> dpi() const noexcept;
};

class DisplayIterator : public std::iterator<std::forward_iterator_tag,
                                              Display,
                                              Display,
                                              const Display*,
                                              const Display&> {
    Display d;
public:
    DisplayIterator() noexcept;
    DisplayIterator(int index) noexcept;

    DisplayIterator& operator++();
    DisplayIterator operator++(int);

    bool operator==(const DisplayIterator &other) const noexcept;
    bool operator!=(const DisplayIterator &other) const noexcept;

    pointer operator->() const noexcept;
    reference operator*() const noexcept;

    friend DisplayIterator begin(DisplayIterator it) noexcept;
    friend DisplayIterator end(DisplayIterator &it) noexcept;
};

}

#endif //SPOOKYSUSHI_DISPLAY_HPP
