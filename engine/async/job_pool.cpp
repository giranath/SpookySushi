#include "job_pool.hpp"

namespace sushi {

job_pool::job_pool(std::size_t size)
: jobs(size)
, allocated_count(0){

}

job* job_pool::allocate() {
    if(!full()) {
        return &jobs[allocated_count++];
    }

    return nullptr;
}

bool job_pool::full() const noexcept {
    return allocated_count == jobs.size();
}

void job_pool::clear() noexcept {
    allocated_count = 0;
}

job* job_pool::make_job(job::job_function fn) {
    job* j = allocate();

    if(j) {
        new(j) job{fn};
    }

    return j;
}

job* job_pool::make_child_job(job::job_function fn, job* parent) {
    job* j = allocate();

    if(j) {
        new(j) job{fn, parent};
    }

    return j;
}

std::size_t job_pool::capacity() const noexcept {
    return jobs.size();
}

std::size_t job_pool::size() const noexcept {
    return allocated_count;
}

}