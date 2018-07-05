#include "key_axis_input_handler.hpp"

namespace sushi {

KeyAxisInputHandler::KeyAxisInputHandler(Key neg, Key pos)
: negative{neg}
, positive{pos}
, negative_pressed{false}
, positive_pressed{false} {

}

bool KeyAxisInputHandler::handle(const InputEvent* event) {
    if(event->type() == InputType::KeyEvent) {
        const KeyEvent* key_event = static_cast<const KeyEvent*>(event);

        if(key_event->key() == negative) {
            negative_pressed = key_event->state() == KeyState::Pressed;
        }
        else if(key_event->key() == positive) {
            positive_pressed = key_event->state() == KeyState::Pressed;
        }
    }

    return false;
}

float KeyAxisInputHandler::value() const {
    if(negative_pressed && !positive_pressed) {
        return -1.0f;
    }
    else if(!negative_pressed && positive_pressed) {
        return 1.0f;
    }
    else {
        return 0.0f;
    }
}


}