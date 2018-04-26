#ifndef SPOOKYSUSHI_WORKER_HPP
#define SPOOKYSUSHI_WORKER_HPP

#include "job.hpp"
#include <cstddef>
#include <thread>
#include <atomic>
#include <chrono>
#include <deque>
#include <mutex>

namespace sushi { namespace async {

class engine;

class worker {
public:
    friend engine;
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
    std::thread::id thread_id;
    std::deque<job*> job_queue;
    std::mutex queue_mutex;

    job* pop_job() noexcept;
    job* next_job() noexcept;
    job* steal() noexcept;

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

    void push(job* j) noexcept;
};

}}


#endif //SPOOKYSUSHI_WORKER_HPP
