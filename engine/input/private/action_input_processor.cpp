#include "action_input_processor.hpp"

namespace sushi {

KeyActionInputProcessor::KeyActionInputProcessor(Key key)
: key{key}
, active{false}
, is_pressed{false} {

}

bool KeyActionInputProcessor::process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) {
    bool is_handled = false;

    std::for_each(begin, end, [this, &is_handled](const InputEvent* event) {
        if(event->type() == InputType::KeyEvent) {
            const KeyEvent* key_event = static_cast<const KeyEvent*>(event);

            if(key_event->key() == key) {
                switch(key_event->state()) {
                    case KeyState::Pressed:
                        if(!is_pressed) {
                            active = true;
                        }
                        is_pressed = true;
                        break;
                    case KeyState::Released:
                        is_pressed = false;
                        break;
                }
                is_handled = true;
            }
        }
    });

    return is_handled;
}

void KeyActionInputProcessor::reset() {
    active = false;
}

bool KeyActionInputProcessor::is_active() const {
    return active;
}

}