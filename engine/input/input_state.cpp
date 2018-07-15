#include "input_state.hpp"
#include <algorithm>

namespace sushi {

InvertedStateInputProcessor::InvertedStateInputProcessor(StateInputProcessor& processor) noexcept
: processor{processor} {

}

bool InvertedStateInputProcessor::is_active() const {
    return !processor.is_active();
}

bool InvertedStateInputProcessor::process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) {
    return processor.process(begin, end);
}

KeyStateInputProcessor::KeyStateInputProcessor(sushi::Key key) noexcept
: key{key}
, is_pressed{false}{

}

bool KeyStateInputProcessor::is_active() const {
    return is_pressed;
}

void KeyStateInputProcessor::set_key(sushi::Key key) noexcept {
    this->key = key;
    is_pressed = false;
}

bool KeyStateInputProcessor::process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) {
    bool is_handled = false;

    std::for_each(begin, end, [this, &is_handled](const InputEvent* event) {
        if(event->type() == InputType::KeyEvent) {
            const KeyEvent* key_event = static_cast<const KeyEvent*>(event);

            if(key_event->key() == key) {
                is_pressed = key_event->state() == KeyState::Pressed;
                is_handled = true;
            }
        }
    });

    return is_handled;
}

MouseStateInputProcessor::MouseStateInputProcessor(MouseButton button) noexcept
: button{button}
, is_pressed{false} {
}

void MouseStateInputProcessor::set_button(MouseButton button) noexcept {
    this->button = button;
    is_pressed = false;
}

bool MouseStateInputProcessor::is_active() const {
    return is_pressed;
}

bool MouseStateInputProcessor::process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) {
    bool is_handled = false;

    std::for_each(begin, end, [this, &is_handled](const InputEvent* event) {
        if(event->type() == InputType::MouseButtonEvent) {
            const MouseButtonEvent* mouse_button_event = static_cast<const MouseButtonEvent*>(event);

            if(mouse_button_event->button() == button) {
                is_pressed = mouse_button_event->state() == MouseButtonState::Down;
                is_handled = true;
            }
        }
    });

    return is_handled;
}

}