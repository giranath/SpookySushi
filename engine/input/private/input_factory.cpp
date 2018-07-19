#include "input_factory.hpp"

#include <algorithm>
#include <iterator>
#include <new>

namespace sushi {

InputFactory::InputFactory() {
    pools[KeyEvent::Type] = {};
    pools[MouseMotionEvent::Type] = {};
    pools[MouseButtonEvent::Type] = {};
}

InputFactory::~InputFactory() {
    for(std::pair<const InputType, Pool> pool : pools) {
        // free memory
        std::for_each(std::begin(pool.second.inputs), std::end(pool.second.inputs), [](InputEvent* event) {
            delete event;
        });
    }
}

const InputEvent* InputFactory::make_key_event(Key key, KeyState state) noexcept {
    return make_event<KeyEvent>(key, state);
}

const InputEvent* InputFactory::make_mouse_button_event(const Vec2& cursor_position,
                                                        MouseButton button, MouseButtonState state,
                                                        uint8_t click_count = 1) noexcept {
    return make_event<MouseButtonEvent>(button, state, cursor_position, click_count);
}

const InputEvent* InputFactory::make_mouse_motion_event(const Vec2& cursor_position, const Vec2& motion) noexcept {
    return make_event<MouseMotionEvent>(cursor_position, motion);
}

void InputFactory::reset() noexcept {
    for(std::pair<const InputType, Pool> pool : pools) {
        pool.second.count = 0;
    }
}

}