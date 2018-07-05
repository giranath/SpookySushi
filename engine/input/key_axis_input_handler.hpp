#ifndef SPOOKYSUSHI_KEY_AXIS_INPUT_HANDLER_HPP
#define SPOOKYSUSHI_KEY_AXIS_INPUT_HANDLER_HPP

#include "input_handler.hpp"

namespace sushi {

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

#endif //SPOOKYSUSHI_KEY_AXIS_INPUT_HANDLER_HPP
