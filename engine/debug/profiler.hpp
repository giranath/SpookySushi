#ifndef SPOOKYSUSHI_PROFILER_HPP
#define SPOOKYSUSHI_PROFILER_HPP

#include "../platform/host_infos.hpp"
#include "../async/scmp_queue.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <array>
#include <atomic>
#include <string>

namespace sushi { namespace debug {

using clock = std::chrono::high_resolution_clock;

// TODO: Add frame index
class profile_event {
public:
    enum class type : uint8_t {
        start,
        end,
        instant
    };
private:
    //std::string name;
    uint32_t name;
    clock::time_point time_point_;
    std::thread::id thread_id;
    type type_;

    static const std::size_t PAYLOAD_SIZE = sizeof(name) + sizeof(time_point_) + sizeof(thread_id) + sizeof(type_);
    static const std::size_t PADDING_SIZE = CACHE_LINE_SIZE - PAYLOAD_SIZE;
    std::array<uint8_t, PADDING_SIZE> padding;
public:
    profile_event() = default;
    profile_event(uint32_t name, type t);
    profile_event(uint32_t name, type t, clock::time_point tp);
    profile_event(uint32_t name, type t, clock::time_point tp, std::thread::id thread_id);

    friend std::ostream& operator<<(std::ostream& os, const profile_event& event);
};

std::ostream& operator<<(std::ostream& os, profile_event::type type);

class profiler {
    std::thread background_thread;
    async::scmp_queue<profile_event> events_queue;
    std::atomic_bool is_running;

    void execute_background_task();

    profiler() noexcept;
public:
    profiler(const profiler&) = delete;
    profiler& operator=(const profiler&) = delete;

    static profiler& get() noexcept;

    void start() noexcept;
    void stop() noexcept;

    bool push(const profile_event& event);
};

class scoped_profile {
    uint32_t name;
public:
    scoped_profile(const scoped_profile&) = delete;
    scoped_profile& operator=(const scoped_profile&) = delete;
    scoped_profile(scoped_profile&&) = delete;
    scoped_profile& operator=(scoped_profile&&) = delete;

    scoped_profile(uint32_t name) noexcept;
    ~scoped_profile() noexcept;
};

}}

#endif //SPOOKYSUSHI_PROFILER_HPP
