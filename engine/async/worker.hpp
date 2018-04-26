#ifndef SPOOKYSUSHI_WORKER_HPP
#define SPOOKYSUSHI_WORKER_HPP

#include "job.hpp"
#include <cstddef>
#include <thread>
#include <atomic>
#include <chrono>

namespace sushi { namespace async {

class worker {
public:
    enum class mode {
        background,
        foreground
    };
    enum class state {
        idle,
        running,
        stopping
    };
private:
    std::atomic<mode> mode_;
    std::atomic<state> state_;
    std::thread thread;

    job* next_job() noexcept;
    void execute_next_job() noexcept;
    void run() noexcept;
public:
    explicit worker(mode m = mode::background);
    ~worker() noexcept;

    mode current_mode() const noexcept;

    void start() noexcept;
    void stop() noexcept;

    bool running() const noexcept;

    // Wait indefinitely
    bool wait_for(job* j) noexcept;

    // Wait with timeout
    bool wait_for(job* j, std::chrono::high_resolution_clock::duration timeout) noexcept;
};

}}


#endif //SPOOKYSUSHI_WORKER_HPP
