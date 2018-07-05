#include "mouse_axis_input_handler.hpp"

namespace sushi {

MouseAxisInputHandler::MouseAxisInputHandler(MouseAxis axis)
: axis{axis}
, delta{0.f} {

}

bool MouseAxisInputHandler::handle(const InputEvent* event) {
    if(event->type() == InputType::MouseMotionEvent) {
        const MouseMotionEvent* mouse_motion_event = static_cast<const MouseMotionEvent*>(event);

        switch(axis) {
            case MouseAxis::Horizontal:
                delta += mouse_motion_event->relative_motion().x;
                break;
            case MouseAxis::Vertical:
                delta += mouse_motion_event->relative_motion().y;
                break;
        }

        return true;
    }

    return false;
}

float MouseAxisInputHandler::value() const {
    return delta;
}

void MouseAxisInputHandler::reset() {
    InputHandler::reset();
    delta = 0.f;
}

}