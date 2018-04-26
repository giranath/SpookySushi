#include "worker.hpp"

namespace sushi { namespace async {

job* worker::next_job() noexcept {
    return nullptr;
}

void worker::execute_next_job() noexcept {
    job* next = next_job();

    if(next) {
        next->execute();
    }
}

void worker::run() noexcept {
    while(running()) {
        execute_next_job();
    }
}

worker::worker(mode m)
: mode_(m)
, state_(state::idle)
, thread{}{

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
    }
}

void worker::stop() noexcept {
    state_ = state::stopping;
}

bool worker::running() const noexcept {
    return state_ == state::running;
}

bool worker::wait_for(job* j) noexcept {
    while(!j->is_finished()) {
        execute_next_job();
    }

    return true;
}

bool worker::wait_for(job* j, std::chrono::high_resolution_clock::duration timeout) noexcept {
    const std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    while(!j->is_finished() && std::chrono::high_resolution_clock::now() - start < timeout) {
        execute_next_job();
    }

    return j->is_finished();
}

}}