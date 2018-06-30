#ifndef SPOOKYSUSHI_INPUT_HANDLER_HPP
#define SPOOKYSUSHI_INPUT_HANDLER_HPP

#include "input_event.hpp"

namespace sushi {

class InputEvent;

class InputHandler {
public:
    virtual bool handle(const InputEvent* event) = 0;
};

//
// State input 0 or 1
//
class StateInputHandler : public InputHandler {
public:
    virtual bool is_active() const = 0;
};

class KeyStateInputHandler : public StateInputHandler {
    Key key;
    bool state;
public:
    KeyStateInputHandler(Key key);
    bool handle(const InputEvent* event) override;
    bool is_active() const override;
};

//
// Axis input -1 to 1
//
class AxisInputHandler : public InputHandler {
public:
    virtual float value() const = 0;
};

class KeyAxisInputHandler : public AxisInputHandler {
    Key negative;
    Key positive;
    bool positive_pressed;
    bool negative_pressed;
public:
    KeyAxisInputHandler(Key neg, Key pos);

    bool handle(const InputEvent* event) override;
    float value() const override;
};

}


#endif //SPOOKYSUSHI_INPUT_HANDLER_HPP
