#ifndef SPOOKYSUSHI_PROFILER_HPP
#define SPOOKYSUSHI_PROFILER_HPP

#include "../../platform/public/host_infos.hpp"
#include "scmp_queue.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <array>
#include <atomic>
#include <string>

namespace sushi { namespace debug {

using Clock = std::chrono::high_resolution_clock;

// TODO: Add frame index
class ProfileEvent {
public:
    enum class Type : uint8_t {
        start,
        end,
        instant
    };
private:
    uint32_t name;
    Clock::time_point time_point_;
    std::thread::id thread_id;
    Type type_;
    uint64_t frame_index;

    static const std::size_t PAYLOAD_SIZE = sizeof(uint32_t) + sizeof(Clock::time_point) + sizeof(std::thread::id) + sizeof(Type) + sizeof(uint64_t);
    static const std::size_t PADDING_SIZE = CACHE_LINE_SIZE - PAYLOAD_SIZE;
    std::array<uint8_t, PADDING_SIZE> padding;
public:
    ProfileEvent() = default;
    ProfileEvent(uint32_t name, Type t, uint64_t frame_index);
    ProfileEvent(uint32_t name, Type t, Clock::time_point tp, uint64_t frame_index);
    ProfileEvent(uint32_t name, Type t, Clock::time_point tp, std::thread::id thread_id, uint64_t frame_index);

    friend std::ostream& operator<<(std::ostream& os, const ProfileEvent& event);
};

std::ostream& operator<<(std::ostream& os, ProfileEvent::Type type);

class Profiler {
    std::thread background_thread;
    async::scmp_queue<ProfileEvent> events_queue;
    std::atomic_bool is_running;
    std::uint64_t current_frame_index;

    void execute_background_task();

    Profiler() noexcept;
public:
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    static Profiler& get() noexcept;

    void start() noexcept;
    void stop() noexcept;

    bool push(const ProfileEvent& event);

    bool push_start(uint32_t name);
    bool push_end(uint32_t name);
    bool push_instant(uint32_t name);

    void set_frame_index(uint64_t current_frame_index) noexcept;
};

class ScopedProfile {
    uint32_t name;
public:
    ScopedProfile(const ScopedProfile&) = delete;
    ScopedProfile& operator=(const ScopedProfile&) = delete;
    ScopedProfile(ScopedProfile&&) = delete;
    ScopedProfile& operator=(ScopedProfile&&) = delete;

    ScopedProfile(uint32_t name) noexcept;
    ~ScopedProfile() noexcept;
};

}}

#endif //SPOOKYSUSHI_PROFILER_HPP
