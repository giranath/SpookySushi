#ifndef SPOOKYSUSHI_EVENT_HPP
#define SPOOKYSUSHI_EVENT_HPP

#include <functional>
#include <array>
#include <mutex>

namespace sushi {

template<typename... Args>
class event {
public:
    using handler_fn = std::function<void(Args...)>;
    using handler_handle = uint16_t;

    static constexpr const handler_handle not_found = std::numeric_limits<handler_handle>::max();
private:
    static constexpr const std::size_t MAX_HANDLER_COUNT = 128;
    std::array<handler_fn, MAX_HANDLER_COUNT> handlers;
    std::array<handler_handle, MAX_HANDLER_COUNT> handles;
    std::size_t count;
    std::mutex mutex;
    handler_handle next_handle;

public:
    constexpr event()
    : count(0)
    , next_handle(0){
    }

    template<typename FN>
    handler_handle attach(FN&& fn) noexcept {
        std::lock_guard<std::mutex> lock(mutex);

        if(count < MAX_HANDLER_COUNT) {
            const handler_handle  handle = next_handle;

            handlers[count] = std::forward<FN>(fn);
            handles[count] = handle;

            ++count;
            ++next_handle;

            return handle;
        }

        return not_found;
    }

    bool is_attached(handler_handle handle) noexcept {
        if(handle != not_found) {
            for(std::size_t i = 0; i < count; ++i) {
                if(handles[i] == handle) {
                    return true;
                }
            }
        }

        return false;
    }

    bool detach(handler_handle handle) noexcept {
        if(handle != not_found) {
            for(std::size_t i = 0; i < count; ++i) {
                if(handles[i] == handle) {
                    using std::swap;
                    swap(handles[i], handles[count - 1]);
                    swap(handlers[i], handlers[count - 1]);

                    --count;

                    return true;
                }
            }
        }

        return false;
    }

    void operator()(Args&&... arguments) {
        std::lock_guard<std::mutex> lock(mutex);
        // Calls every stored handlers
        for(std::size_t i = 0; i < count; ++i) {
            handlers[i](std::forward<Args>(arguments)...);
        }
    }
};

}

#endif //SPOOKYSUSHI_EVENT_HPP
