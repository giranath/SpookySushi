#ifndef SPOOKYSUSHI_MOUSE_AXIS_INPUT_HANDLER_HPP
#define SPOOKYSUSHI_MOUSE_AXIS_INPUT_HANDLER_HPP

#include "input_handler.hpp"

namespace sushi {

enum class MouseAxis : uint8_t {
    Vertical,
    Horizontal
};

// does not work
class MouseAxisInputHandler : public AxisInputHandler {
    MouseAxis axis;
    float delta;
public:
    explicit MouseAxisInputHandler(MouseAxis axis);
    bool handle(const InputEvent *event) override;
    float value() const override;

    void reset() override;

};

}

#endif
