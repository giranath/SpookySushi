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
private:
    static constexpr const std::size_t MAX_HANDLER_COUNT = 128;
    std::array<handler_fn, MAX_HANDLER_COUNT> handlers;
    std::size_t count;
    std::mutex mutex;

public:
    template<typename FN>
    bool attach(FN&& fn) noexcept {
        std::lock_guard<std::mutex> lock(mutex);

        if(count < MAX_HANDLER_COUNT) {
            handlers[count] = std::forward<FN>(fn);
            ++count;

            return true;
        }

        return false;
    }

    template<typename FN>
    bool is_attached(const FN& fn) noexcept {
        for(std::size_t i = 0; i < count; ++i) {
            if(&handlers[i] == &fn) {
                return true;
            }
        }

        return false;
    }

    template<typename FN>
    void detach(const FN& fn) noexcept {
        for(std::size_t i = 0; i < count; ++i) {
            if(&handlers[i] == &fn) {
                std::swap(handlers[i], handlers[count - 1]);
                --count;
            }
        }
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
