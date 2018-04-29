#ifndef SPOOKYSUSHI_WORKER_HPP
#define SPOOKYSUSHI_WORKER_HPP

#include "job.hpp"
#include "job_pool.hpp"

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
    engine& owner;
    std::atomic<mode> mode_;
    std::atomic<state> state_;
    std::thread thread;
    std::thread::id thread_id;
    std::deque<job*> job_queue;
    std::mutex queue_mutex;
    job_pool jobs_;

    job* pop_job() noexcept;
    job* next_job() noexcept;
    job* steal() noexcept;

    void execute_next_job() noexcept;
    void run() noexcept;
public:
    worker(engine& owner, std::size_t max_job_count, mode m = mode::background);
    ~worker() noexcept;

    mode current_mode() const noexcept;

    void start() noexcept;
    void stop() noexcept;

    bool running() const noexcept;

    // Wait indefinitely
    bool wait_for(job* j) noexcept;

    // Wait with timeout
    // Could wait more than timeout
    bool wait_for(job* j, std::chrono::high_resolution_clock::duration timeout) noexcept;

    void wait(std::chrono::high_resolution_clock::duration timeout) noexcept;

    void push(job* j) noexcept;

    job_pool& pool() noexcept;

    job* make_job(job::job_fn fn, job* parent = nullptr) noexcept;

    template<typename Data>
    job* make_job(job::job_fn fn, const Data& data, job* parent = nullptr) noexcept {
        static_assert(std::is_pod<Data>::value, "data must be a POD type");

        job* j = pool().allocate();

        if(j) {
            new(j) job(fn, parent);
            j->store<Data>(data);
            push(j);
        }

        return j;
    }

    template<typename Closure>
    job* make_closure(Closure closure, job* parent = nullptr) noexcept {
        job* j = pool().allocate();

        if(j) {
            job::make_closure(j, closure);
            push(j);
        }

        return j;
    }
};

}}


#endif //SPOOKYSUSHI_WORKER_HPP
