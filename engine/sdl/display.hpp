#ifndef SPOOKYSUSHI_DISPLAY_HPP
#define SPOOKYSUSHI_DISPLAY_HPP

#include "../core/rect.hpp"

#include <iterator>

namespace sushi {

class display_iterator;

class display {
    friend display_iterator;
    int index;

public:
    struct dpi_infos {
        float diagonal;
        float horizontal;
        float vertical;

        dpi_infos(float d, float h, float v)
        : diagonal{d}, horizontal{h}, vertical{v} {

        }
    };

    explicit display(int index) noexcept;

    recti bounds() const noexcept;
    recti usable_bounds() const noexcept;
    const char* name() const noexcept;

    bool try_get_dpi(dpi_infos& infos) const noexcept;
};

class display_iterator : public std::iterator<std::forward_iterator_tag,
                                              display,
                                              display,
                                              const display*,
                                              const display&> {
    display d;
public:
    display_iterator() noexcept;
    display_iterator(int index) noexcept;

    display_iterator& operator++();
    display_iterator operator++(int);

    bool operator==(const display_iterator &other) const noexcept;
    bool operator!=(const display_iterator &other) const noexcept;

    pointer operator->() const noexcept;
    reference operator*() const noexcept;

    friend display_iterator begin(display_iterator it) noexcept;
    friend display_iterator end(display_iterator &it) noexcept;
};

}

#endif //SPOOKYSUSHI_DISPLAY_HPP
