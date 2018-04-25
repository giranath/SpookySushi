#ifndef SPOOKYSUSHI_JOB_POOL_HPP
#define SPOOKYSUSHI_JOB_POOL_HPP

#include "job.hpp"
#include <vector>

namespace sushi {

class job_pool {
    std::vector<job> jobs;
    std::size_t allocated_count;
public:
    explicit job_pool(std::size_t size);

    job* allocate();
    bool full() const noexcept;
    void clear() noexcept;

    job* make_job(job::job_function fn);
    job* make_child_job(job::job_function fn, job* parent);

    template<typename Data>
    job* make_job(job::job_function fn, const Data& data) {
        return make_child_job(fn, nullptr, data);
    }

    template<typename Data>
    job* make_child_job(job::job_function fn, job* parent, const Data& data) {
        job* j = make_child_job(fn, parent);

        if(j) {
            j->set_data(data);
        }

        return j;
    }

    template<typename Function>
    job* make_closure_job(Function fn) {
        return make_child_closure_job(fn, nullptr);
    }

    template<typename Function>
    job* make_child_closure_job(Function fn, job* parent) {
        job* j = allocate();

        if(j) {
            job::make_closure(j, fn, parent);
        }

        return j;
    }

    std::size_t capacity() const noexcept;
    std::size_t size() const noexcept;
};

}


#endif //SPOOKYSUSHI_JOB_POOL_HPP
