#include "key_state_input_handler.hpp"

namespace sushi {

KeyStateInputHandler::KeyStateInputHandler(Key key)
: key{key}
, state{false} {

}

bool KeyStateInputHandler::handle(const InputEvent* event) {
    if(event->type() == InputType::KeyEvent) {
        const KeyEvent* key_event = static_cast<const KeyEvent*>(event);

        if(key_event->key() == key) {
            state = key_event->state() == KeyState::Pressed;
            return true;
        }
    }

    return false;
}

bool KeyStateInputHandler::is_active() const {
    return state;
}

}