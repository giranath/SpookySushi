#ifndef SPOOKYSUSHI_JOB_HPP
#define SPOOKYSUSHI_JOB_HPP

#include "../platform/host_infos.hpp"
#include <atomic>
#include <cstring>
#include <new>
#include <memory>

namespace sushi { namespace async {

class job {
public:
    using job_fn = void(*)(job&);
private:
    job_fn fn;
    job* parent;
    std::atomic_size_t unfinished_count;

    // Add some padding to disable False-Sharing
    static const std::size_t PAYLOAD_SIZE = sizeof(fn) + sizeof(parent) + sizeof(unfinished_count);
    static const std::size_t PADDING_SIZE = sushi::CACHE_LINE_SIZE - PAYLOAD_SIZE;
    uint8_t padding[PADDING_SIZE];

    void terminate() noexcept;
public:
    constexpr job() noexcept
    : fn(nullptr)
    , parent(nullptr)
    , unfinished_count(0)
    , padding{0} {

    }

    constexpr job(job_fn fn) noexcept
    : fn(fn)
    , parent(nullptr)
    , unfinished_count(1)
    , padding{0}{

    }

    constexpr job(job_fn fn, job* parent) noexcept
    : fn(fn)
    , parent(parent)
    , unfinished_count(1)
    , padding{0} {
        if(parent) {
            parent->unfinished_count++;
        }
    }

    void execute();

    bool is_finished() const noexcept;

    // Can only store small pod data
    template<typename Data, typename Allocator = std::allocator<Data>>
    typename std::enable_if<std::is_pod<Data>::value && (sizeof(Data) <= PADDING_SIZE)>::type
    store(const Data& data) noexcept {
        std::memcpy(padding, &data, sizeof(data));
    }

    // Construct an object in place
    template<typename Type, typename... Args>
    typename std::enable_if<sizeof(Type) <= PADDING_SIZE>::type
    emplace(Args... args) {
        new(padding) Type(std::forward<Args>(args)...);
    }

    // Retrieve stored data
    template<typename Data>
    Data& data() noexcept {
        return *reinterpret_cast<Data*>(padding);
    }

    template<typename Data>
    const Data& data() const noexcept {
        return *reinterpret_cast<Data*>(padding);
    }

    template<typename Closure, typename Allocator = std::allocator<Closure>>
    static typename std::enable_if<(sizeof(Closure) <= PADDING_SIZE), job*>::type
    make_closure(job* j, job* parent, Closure closure) {
        auto fn = [](job& ze_job) {
            Closure& closure = ze_job.data<Closure>();

            // Execute the function
            closure(ze_job);

            // Calls the destructor of the attached closure
            closure.~Closure();
        };

        new(j) job(fn, parent);
        j->emplace<Closure>(closure);

        return j;
    }

    template<typename Closure, typename Allocator = std::allocator<Closure>>
    static typename std::enable_if<(sizeof(Closure) > PADDING_SIZE), job*>::type
    make_closure(job* j, job* parent, Closure closure) {
        struct closure_data {
            Closure* closure;
            Allocator allocator;

            closure_data(Closure* closure, const Allocator& allocator)
            : closure(closure)
            , allocator(allocator) {

            }
        };

        auto fn = [](job& ze_job) {
            auto data = ze_job.data<closure_data>();

            (*data.closure)(ze_job);

            // Calls destructor
            data.closure->~Closure();
            data.allocator.deallocate(data.closure, 1);
        };

        new(j) job(fn, parent);

        Allocator allocator;
        Closure* ptr_location = allocator.allocate(1);
        new(ptr_location) Closure{closure};

        j->emplace<closure_data>(ptr_location, allocator);

        return j;
    }

    template<typename Closure, typename Allocator = std::allocator<Closure>>
    static job* make_closure(job* j, Closure closure) {
        return make_closure(j, nullptr, std::move(closure));
    }
};

}}

#endif //SPOOKYSUSHI_JOB_HPP
