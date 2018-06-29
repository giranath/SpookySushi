#ifndef SPOOKYSUSHI_INPUT_EVENT_HPP
#define SPOOKYSUSHI_INPUT_EVENT_HPP

#include "../core/types.hpp"
#include <cstdint>
#include <chrono>

namespace sushi {

using InputClock = std::chrono::high_resolution_clock;
using InputTimePoint = typename InputClock::time_point;

enum class InputType : uint8_t {
    KeyEvent,
    MouseButtonEvent,
    MouseMotionEvent
};

class InputEvent {
    InputTimePoint time_point_;
    InputType type_;
public:
    InputEvent(InputType type) noexcept;
    InputEvent(InputType type, InputTimePoint timepoint) noexcept;

    InputType type() const noexcept;
    InputTimePoint time_point() const noexcept;
};

// Keyboard related events

enum class Key : uint8_t {
    W,
    A,
    S,
    D,
    Unsupported
};

enum class KeyState : uint8_t {
    Pressed,
    Released
};

class KeyEvent : public InputEvent {
    Key key_;
    KeyState state_;
public:
    constexpr static const InputType Type = InputType::KeyEvent;

    KeyEvent(Key key, KeyState state) noexcept;

    Key key() const noexcept;
    KeyState state() const noexcept;
};

// Mouse related events

enum class MouseButton : uint8_t {
    Left,
    Right,
    Middle,
    Unsupported
};

enum class MouseButtonState : uint8_t {
    Down,
    Up
};

class MouseEvent : public InputEvent {
    Vec2 cursor_position_;
public:
    MouseEvent(InputType type, Vec2 cursor_position) noexcept;

    const Vec2& cursor_position() const noexcept;
};

class MouseMotionEvent : public MouseEvent {
    Vec2 relative_motion_;
public:
    constexpr static const InputType Type = InputType::MouseMotionEvent;

    MouseMotionEvent(Vec2 cursor_position, Vec2 relative_motion) noexcept;

    const Vec2& relative_motion() const noexcept;
};

class MouseButtonEvent : public MouseEvent {
    MouseButton button_;
    MouseButtonState state_;
    uint8_t click_count_;
public:
    constexpr static const InputType Type = InputType::MouseButtonEvent;

    MouseButtonEvent(MouseButton button, MouseButtonState state, const Vec2& cursor_position) noexcept;
    MouseButtonEvent(MouseButton button, MouseButtonState state,
                     const Vec2& cursor_position, uint8_t click_count) noexcept;
    MouseButton button() const noexcept;
    MouseButtonState state() const noexcept;
    uint8_t click_count() const noexcept;
};

}

#endif //SPOOKYSUSHI_INPUT_EVENT_HPP
