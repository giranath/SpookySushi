#include "axis_input_processor.hpp"

#include <algorithm>

namespace sushi {

KeyAxisInputProcessor::KeyAxisInputProcessor(Key positive, Key negative) noexcept
: positive_key{positive}
, negative_key{negative}
, is_positive_pressed{false}
, is_negative_pressed{false} {

}

void KeyAxisInputProcessor::set_keys(Key positive, Key negative) noexcept {
    positive_key = positive;
    negative_key = negative;
}

void KeyAxisInputProcessor::set_positive_key(Key key) noexcept {
    positive_key = key;
}

void KeyAxisInputProcessor::set_negative_key(Key key) noexcept {
    negative_key = key;
}

bool sushi::KeyAxisInputProcessor::process(sushi::InputBusReader::const_iterator begin,
                                           sushi::InputBusReader::const_iterator end) {
    bool is_handled = false;

    std::for_each(begin, end, [this, &is_handled](const InputEvent* event) {
        if(event->type() == InputType::KeyEvent) {
            const KeyEvent* key_event = static_cast<const KeyEvent*>(event);

            if(key_event->key() == positive_key) {
                is_positive_pressed = key_event->state() == KeyState::Pressed;
                is_handled = true;
            }
            else if(key_event->key() == negative_key) {
                is_negative_pressed = key_event->state() == KeyState::Pressed;
                is_handled = true;
            }
        }
    });

    return is_handled;
}

float sushi::KeyAxisInputProcessor::value() const {
    if (is_positive_pressed && is_negative_pressed) {
        return 0.f;
    } else if (is_positive_pressed) {
        return 1.f;
    } else if(is_negative_pressed) {
        return -1.f;
    }
    else {
        return 0.f;
    }
}

MouseAxisInputProcessor::MouseAxisInputProcessor(MouseAxis axis, float sensibility) noexcept
: axis{axis}
, sensibility{sensibility} {

}

bool MouseAxisInputProcessor::process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) {
    bool is_handled = false;

    current_value = 0.f;
    std::for_each(begin, end, [this, &is_handled](const InputEvent* event) {
       if(event->type() == InputType::MouseMotionEvent) {
           const MouseMotionEvent* motion_event = static_cast<const MouseMotionEvent*>(event);

           switch(axis) {
               case MouseAxis::Horizontal:
                   current_value += motion_event->relative_motion().x;
                   break;
               case MouseAxis::Vertical:
                   current_value += motion_event->relative_motion().y;
                   break;
           }

           is_handled = true;
       }
    });

    return is_handled;
}

float MouseAxisInputProcessor::value() const {
    return current_value * sensibility;
}

bool MouseAxisInputProcessor::is_normalized() const {
    return false;
}

void MouseAxisInputProcessor::set_sensibility(float new_sensibility) noexcept {
    sensibility = new_sensibility;
}

}