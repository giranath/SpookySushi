#ifndef SPOOKYSUSHI_INPUT_STATE_HPP
#define SPOOKYSUSHI_INPUT_STATE_HPP

#include "input_event.hpp"
#include "input_bus.hpp"
#include "input_processor.hpp"

namespace sushi {

class InvertedStateInputProcessor : public StateInputProcessor {
    StateInputProcessor& processor;
public:
    explicit InvertedStateInputProcessor(StateInputProcessor& processor) noexcept;
    bool is_active() const override;
    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
};

class KeyStateInputProcessor : public StateInputProcessor {
    Key key;
    bool is_pressed;
public:
    explicit KeyStateInputProcessor(Key key) noexcept;
    void set_key(Key key) noexcept;
    bool is_active() const override;
    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
};

class MouseStateInputProcessor : public StateInputProcessor {
    MouseButton button;
    bool is_pressed;
public:
    explicit MouseStateInputProcessor(MouseButton button) noexcept;
    void set_button(MouseButton button) noexcept;
    bool is_active() const override;
    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
};

}

#endif
