#include "engine.hpp"
#include <iterator>
#include <algorithm>
#include <cassert>

namespace sushi { namespace async {

engine::engine(std::size_t background_count)
: workers{}
, background_roundrobin(1)
, rd{static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count())}{
    workers.push_back(std::make_unique<worker>(worker::mode::foreground));
    for(std::size_t i = 0; i < background_count; ++i) {
        workers.push_back(std::make_unique<worker>(worker::mode::background));
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
    std::size_t next_id = background_roundrobin.load(std::memory_order_acquire);

    assert(next_id >= 1);
    assert(next_id < workers.size());

    worker* w = workers[next_id].get();

    next_id = next_id + 1;
    if(next_id >= workers.size()) {
        next_id = 1;
    }

    background_roundrobin.store(next_id, std::memory_order_release);

    return w;
}

const worker* engine::next_background() const noexcept {
    std::size_t next_id = background_roundrobin.load(std::memory_order_acquire);

    worker* w = workers[next_id].get();

    next_id = next_id + 1;
    if(next_id > workers.size()) {
        next_id = 1;
    }

    background_roundrobin.store(next_id, std::memory_order_release);

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

}}