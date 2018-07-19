#ifndef SPOOKYSUSHI_INPUT_POOL_HPP
#define SPOOKYSUSHI_INPUT_POOL_HPP

#include "input_event.hpp"
#include <unordered_map>
#include <vector>

namespace sushi {

class InputFactory {
    struct Pool {
        std::vector<InputEvent*> inputs;
        std::vector<InputEvent*>::size_type count;
    };

    std::unordered_map<InputType, Pool> pools;

    template<typename EventType, typename... Args>
    const InputEvent* make_event(Args... args) noexcept {
        Pool& pool = pools[EventType::Type];
        InputEvent* event = nullptr;

        // Need to create a new input
        if(pool.count >= pool.inputs.size()) {
            event = new EventType(std::forward<Args>(args)...);
            pool.inputs.push_back(event);
        }
            // Reuse an old event
        else {
            event = new(pool.inputs[pool.count]) EventType(std::forward<Args>(args)...);
        }

        ++pool.count;

        return event;
    }

public:
    InputFactory();
    ~InputFactory();
    const InputEvent* make_key_event(Key key, KeyState state) noexcept;
    const InputEvent* make_mouse_button_event(const Vec2& cursor_position,
                                              MouseButton button, MouseButtonState state,
                                              uint8_t click_count) noexcept;
    const InputEvent* make_mouse_motion_event(const Vec2& cursor_position, const Vec2& motion) noexcept;

    void reset() noexcept;
};

}

#endif //SPOOKYSUSHI_INPUT_POOL_HPP
