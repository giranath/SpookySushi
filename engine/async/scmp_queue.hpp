#ifndef SPOOKYSUSHI_SCMP_QUEUE_HPP
#define SPOOKYSUSHI_SCMP_QUEUE_HPP

#include <vector>
#include <mutex>
#include <cassert>
#include <atomic>

namespace sushi { namespace async {

// Single Consumer Multiple Producers queue
template<typename T>
class scmp_queue {
    std::vector<T> data;
    std::size_t head_;
    std::size_t tail_;
    mutable std::mutex mutex_;
public:
    scmp_queue() noexcept
    : data{}
    , head_{}
    , tail_{} {

    }

    explicit scmp_queue(std::size_t max_count)
    : data{max_count}
    , head_(0)
    , tail_(0) {

    }

    // Can only be called by the producers
    bool push(const T& t) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);

        if(tail_ - head_ < data.size()) {
            data[tail_] = t;
            ++tail_;

            return true;
        }

        return false;
    }

    // Can only be called by the single consumer
    void pop() noexcept {
        std::lock_guard<std::mutex> lock(mutex_);

        ++head_;
    }

    T& front() noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return data[head_ % data.size()];
    }

    const T& front() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return data[head_ % data.size()];
    }

    bool empty() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return head_ >= tail_;
    }

    bool full() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return tail_ - head_ >= data.size();
    }

    void clear() noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        head_ = 0;
        tail_ = 0;
    }

    template<typename OutputIt>
    OutputIt transfert(OutputIt it) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        for(std::size_t i = head_; i < tail_; ++i) {
            *it = data[i % data.size()];
            ++it;
        }

        // Clear the content
        head_ = 0;
        tail_ = 0;

        return it;
    }
};

}}

#endif
