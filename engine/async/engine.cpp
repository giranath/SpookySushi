#include "engine.hpp"
#include <iterator>
#include <algorithm>
#include <cassert>

namespace sushi { namespace async {

engine::engine(std::size_t background_count, std::size_t max_job_per_worker)
: workers{}
, background_roundrobin(1)
, rd{static_cast<std::default_random_engine::result_type>(std::chrono::system_clock::now().time_since_epoch().count())}{
    workers.push_back(std::make_unique<worker>(*this, max_job_per_worker, worker::mode::foreground));
    for(std::size_t i = 0; i < background_count; ++i) {
        workers.push_back(std::make_unique<worker>(*this, max_job_per_worker, worker::mode::background, i + 1));
    }
}

void engine::start() noexcept {
    for(auto it = std::begin(workers); it != std::end(workers); ++it) {
        (*it)->start();
    }
}

void engine::stop() noexcept {
    for(auto it = std::begin(workers); it != std::end(workers); ++it) {
        (*it)->stop();
    }
}

worker* engine::foreground() noexcept {
    assert(!workers.empty());
    return workers.front().get();
}

const worker* engine::foreground() const noexcept {
    assert(!workers.empty());
    return workers.front().get();
}

worker* engine::random_background() noexcept {
    assert(workers.size() < std::numeric_limits<int>::max());

    std::uniform_int_distribution<int> dist(1, static_cast<int>(workers.size() - 1));

    return workers[dist(rd)].get();
}

const worker* engine::random_background() const noexcept {
    assert(workers.size() < std::numeric_limits<int>::max());

    std::uniform_int_distribution<int> dist(1, static_cast<int>(workers.size() - 1));

    return workers[dist(rd)].get();
}

worker* engine::next_background() noexcept {
    std::lock_guard<std::mutex> lock(roundrobin_mutex);

    assert(background_roundrobin >= 1);
    assert(background_roundrobin < workers.size());

    worker* w = workers[background_roundrobin].get();

    ++background_roundrobin;
    if(background_roundrobin >= workers.size()) {
        background_roundrobin = 1;
    }

    return w;
}

const worker* engine::next_background() const noexcept {
    std::lock_guard<std::mutex> lock(roundrobin_mutex);

    assert(background_roundrobin >= 1);
    assert(background_roundrobin < workers.size());

    worker* w = workers[background_roundrobin].get();

    ++background_roundrobin;
    if(background_roundrobin >= workers.size()) {
        background_roundrobin = 1;
    }

    return w;
}

// Returns the current thread associated worker
worker* engine::current() noexcept {
    const std::thread::id current_id = std::this_thread::get_id();

    for(auto it = std::begin(workers); it != std::end(workers); ++it) {
        if(it->get()->thread_id == current_id) {
            return it->get();
        }
    }

    return nullptr;
}

const worker* engine::current() const noexcept {
    const std::thread::id current_id = std::this_thread::get_id();

    for(auto it = std::begin(workers); it != std::end(workers); ++it) {
        if(it->get()->thread_id == current_id) {
            return it->get();
        }
    }

    return nullptr;
}

void engine::clear_pools() noexcept {
    for(auto it = std::begin(workers); it != std::end(workers); ++it) {
        (*it)->pool().clear();
    }
}

job* engine::make_job(worker::mode mode, job::job_fn fn, job* parent) noexcept {
    worker* w = nullptr;
    if(mode == worker::mode::foreground) {
        w = workers.front().get();
    }
    else {
        w = next_background();
    }

    return w->make_job(fn, parent);
}

}}