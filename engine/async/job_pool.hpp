#ifndef SPOOKYSUSHI_JOB_POOL_HPP
#define SPOOKYSUSHI_JOB_POOL_HPP

#include "job.hpp"
#include <vector>

namespace sushi { namespace async {

class job_pool {
    std::vector<job> jobs_;
    std::size_t next_id;
public:
    explicit job_pool(std::size_t capacity);

    job* allocate() noexcept;

    void clear();

    bool empty() const noexcept;
    std::size_t capacity() const noexcept;
    std::size_t size() const noexcept;
};

}}


#endif //SPOOKYSUSHI_JOB_POOL_HPP
