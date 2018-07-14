#ifndef SPOOKYSUSHI_INPUT_BUS_HPP
#define SPOOKYSUSHI_INPUT_BUS_HPP

#include <vector>

namespace sushi {

class InputEvent;

class InputBus {
    std::vector<const InputEvent*> inputs;
public:
    using iterator = typename std::vector<const InputEvent*>::iterator;
    using const_iterator = typename std::vector<const InputEvent*>::const_iterator;

    void push(const InputEvent* event);

    void clear();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;
};

class InputBusReader {
    InputBus& bus;
public:
    using const_iterator = typename InputBus::const_iterator;

    explicit InputBusReader(InputBus& bus) noexcept;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
};

}

#endif
