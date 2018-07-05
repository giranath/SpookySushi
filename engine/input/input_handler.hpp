#ifndef SPOOKYSUSHI_INPUT_HANDLER_HPP
#define SPOOKYSUSHI_INPUT_HANDLER_HPP

#include "input_event.hpp"

namespace sushi {

class InputEvent;

class InputHandler {
public:
    virtual bool handle(const InputEvent* event) = 0;
    virtual void reset() {}
};

//
// State input 0 or 1
//
class StateInputHandler : public InputHandler {
public:
    virtual bool is_active() const = 0;
};

//
// Axis input -1 to 1
//
class AxisInputHandler : public InputHandler {
public:
    virtual float value() const = 0;
};


}


#endif //SPOOKYSUSHI_INPUT_HANDLER_HPP
