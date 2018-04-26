#include "job_pool.hpp"
#include <cassert>

namespace sushi { namespace async {

job_pool::job_pool(std::size_t capacity)
: jobs_(capacity)
, next_id(0) {

}

job* job_pool::allocate() noexcept {
    if(next_id < static_cast<std::size_t>(jobs_.size())) {
        return &jobs_[next_id++];
    }

    return nullptr;
}

void job_pool::clear() {
    next_id = 0;
}

bool job_pool::empty() const noexcept {
    return size() == 0;
}

std::size_t job_pool::capacity() const noexcept {
    return jobs_.size();
}

std::size_t job_pool::size() const noexcept {
    return next_id;
}

}}