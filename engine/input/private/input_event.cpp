#include "input_event.hpp"

namespace sushi {

InputEvent::InputEvent(InputType type) noexcept
: type_(type)
, time_point_(InputClock::now()){

}

InputEvent::InputEvent(InputType type, InputTimePoint timepoint) noexcept
: type_(type)
, time_point_(timepoint) {

}

InputType InputEvent::type() const noexcept {
    return type_;
}

InputTimePoint InputEvent::time_point() const noexcept {
    return time_point_;
}

KeyEvent::KeyEvent(Key key, KeyState state) noexcept
: InputEvent(InputType::KeyEvent)
, key_(key)
, state_(state) {

}

Key KeyEvent::key() const noexcept {
    return key_;
}

KeyState KeyEvent::state() const noexcept {
    return state_;
}

MouseEvent::MouseEvent(InputType type, Vec2 cursor_position) noexcept
: InputEvent(type)
, cursor_position_(cursor_position){

}

const Vec2& MouseEvent::cursor_position() const noexcept {
    return cursor_position_;
}

MouseMotionEvent::MouseMotionEvent(Vec2 cursor_position, Vec2 relative_motion) noexcept
: MouseEvent(InputType::MouseMotionEvent, cursor_position)
, relative_motion_(relative_motion) {

}

const Vec2& MouseMotionEvent::relative_motion() const noexcept {
    return relative_motion_;
}

MouseButtonEvent::MouseButtonEvent(MouseButton button, MouseButtonState state, const Vec2& cursor_position) noexcept
: MouseEvent(InputType::MouseButtonEvent, cursor_position)
, button_(button)
, state_(state)
, click_count_(1) {

}

MouseButtonEvent::MouseButtonEvent(MouseButton button, MouseButtonState state,
                                   const Vec2& cursor_position, uint8_t click_count) noexcept
: MouseEvent(InputType::MouseButtonEvent, cursor_position)
, button_(button)
, state_(state)
, click_count_(click_count) {

}

MouseButton MouseButtonEvent::button() const noexcept {
    return button_;
}

MouseButtonState MouseButtonEvent::state() const noexcept {
    return state_;
}

uint8_t MouseButtonEvent::click_count() const noexcept {
    return click_count_;
}

}