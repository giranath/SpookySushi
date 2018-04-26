#ifndef SPOOKYSUSHI_ENGINE_HPP
#define SPOOKYSUSHI_ENGINE_HPP

#include "worker.hpp"
#include <cstddef>
#include <vector>
#include <random>
#include <memory>
#include <mutex>

namespace sushi { namespace async {

class engine {
    std::vector<std::unique_ptr<worker>> workers;
    mutable std::size_t background_roundrobin;
    mutable std::mutex roundrobin_mutex;
    mutable std::default_random_engine rd;
public:
    engine(std::size_t background_count, std::size_t max_job_per_worker);

    void start() noexcept;
    void stop() noexcept;

    // Returns the foreground worker
    worker* foreground() noexcept;
    const worker* foreground() const noexcept;

    // Returns a random background worker
    worker* random_background() noexcept;
    const worker* random_background() const noexcept;

    // Returns a background worker (Round robin)
    worker* next_background() noexcept;
    const worker* next_background() const noexcept;

    // Returns the current thread associated worker
    worker* current() noexcept;
    const worker* current() const noexcept;

    void clear_pools() noexcept;
};

}}



#endif //SPOOKYSUSHI_ENGINE_HPP
