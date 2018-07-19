#include "../../platform/public/host_infos.hpp"
#include "worker.hpp"
#include "engine.hpp"

#include <cassert>

namespace sushi { namespace async {

job* worker::pop_job() noexcept {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if(!job_queue.empty()) {
        job* j = job_queue.front();
        job_queue.pop_front();

        return j;
    }

    return nullptr;
}

job* worker::next_job() noexcept {
    job* local_job = pop_job();

    if(local_job) {
        return local_job;
    }

    worker* other = owner.random_background();
    if(other != this) return other->steal();

    return nullptr;
}

job* worker::steal() noexcept {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if(!job_queue.empty()) {
        job* j = job_queue.back();
        job_queue.pop_back();

        return j;
    }

    return nullptr;
}

void worker::execute_next_job() noexcept {
    job* next = next_job();

    if(next) {
        next->execute();
    }
    else {
        std::this_thread::yield();
    }
}

void worker::run() noexcept {
#if SUSHI_SUPPORT_THREAD_AFFINITY
    /*
    if(worker_id >= 0) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(worker_id, &cpuset);
        int rc = pthread_setaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            // TODO: handle error [Log]
        }
    }
     */
#endif

    while(running()) {
        execute_next_job();
    }
}

worker::worker(engine& owner, std::size_t max_job_count, mode m, int worker_id)
: owner(owner)
, mode_(m)
, state_(state::idle)
, thread{}
, jobs_(max_job_count)
, worker_id(worker_id) {

}

worker::~worker() noexcept {
    if(thread.joinable()) {
        thread.join();
    }
}

worker::mode worker::current_mode() const noexcept {
    return mode_;
}

void worker::start() noexcept {
    state_ = state::running;

    if(mode_ == mode::background) {
        thread = std::thread(&worker::run, std::ref(*this));
        thread_id = thread.get_id();
    }
    else {
        thread_id = std::this_thread::get_id();
    }
}

void worker::stop() noexcept {
    state_ = state::stopping;
}

bool worker::running() const noexcept {
    return state_ == state::running;
}

bool worker::wait_for(job* j) noexcept {
    assert(mode_ == mode::foreground);

    while(!j->is_finished()) {
        execute_next_job();
    }

    return true;
}

bool worker::wait_for(job* j, std::chrono::high_resolution_clock::duration timeout) noexcept {
    assert(mode_ == mode::foreground);

    const std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    while(!j->is_finished() && std::chrono::high_resolution_clock::now() - start < timeout) {
        execute_next_job();
    }

    return j->is_finished();
}

void worker::wait(std::chrono::high_resolution_clock::duration timeout) noexcept {
    assert(mode_ == mode::foreground);

    const std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    while(std::chrono::high_resolution_clock::now() - start < timeout) {
        execute_next_job();
    }
}

void worker::push(job* j) noexcept {
    std::lock_guard<std::mutex> lock(queue_mutex);
    job_queue.push_back(j);
}

job_pool& worker::pool() noexcept {
    return jobs_;
}

job* worker::make_job(job::job_fn fn, job* parent) noexcept {
    job* j = pool().allocate();

    if(j) {
        new(j) job(fn, parent);
        push(j);

        return j;
    }

    return nullptr;
}

}}