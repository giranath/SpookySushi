#ifndef SPOOKYSUSHI_JOB_HPP
#define SPOOKYSUSHI_JOB_HPP

#include <array>
#include <atomic>
#include <cstring>

namespace sushi {

// Inspired by https://manu343726.github.io/2017/03/13/lock-free-job-stealing-task-system-with-modern-c.html

class job {
public:
    using job_function = void(*)(job&);
private:
    job_function function = nullptr;
    job* parent = nullptr;
    std::atomic_size_t unfinish_count = 0;

    static constexpr std::size_t PAYLOAD_SIZE = sizeof(function) + sizeof(parent) + sizeof(unfinish_count);
    static constexpr std::size_t MAX_PADDING_SIZE = 64; // Padding to avoid False sharing
    static constexpr std::size_t PADDING_SIZE = MAX_PADDING_SIZE - PAYLOAD_SIZE;
    std::array<unsigned char, PADDING_SIZE> padding;

    void finish();
public:
    job() = default;
    explicit job(job_function fn, job* parent = nullptr);

    template<typename Data>
    job(job_function fn, const Data& data, job* parent = nullptr)
    : job(fn, parent) {
        set_data(data);
    }

    void run();
    bool finished();

    // Can only set POD data with this method
    template<typename Data>
    typename std::enable_if<std::is_pod<Data>::value && (sizeof(Data) <= PADDING_SIZE)>::type
    set_data(const Data& data) noexcept {
        std::memcpy(padding.data(), &data, sizeof(Data));
    }

    template<typename Data>
    const Data& data() const noexcept  {
        return *reinterpret_cast<const Data*>(padding.data());
    }

    template<typename T, typename... Args>
    typename std::enable_if<(sizeof(T) <= PADDING_SIZE)>::type
    construct_data(Args&&... arguments) {
        new(padding.data())T(std::forward<Args>(arguments)...);
    }

    template<typename Function>
    static typename std::enable_if<(sizeof(Function) <= PADDING_SIZE)>::type make_closure(job* j, Function fn, job* parent = nullptr) {
        auto t = sizeof(Function);
        auto job_fn = [](job& the_job) {
            const auto& function = the_job.data<Function>();
            function(the_job);

            // Calls the destructor
            function.~Function();
        };

        new(j) job(job_fn, parent);
        j->construct_data<Function>(fn);
    }

    template<typename Function>
    static typename std::enable_if<(sizeof(Function) > PADDING_SIZE)>::type make_closure(job* j, Function fn, job* parent = nullptr) {
        auto job_fn = [](job& the_job) {
            Function* function = the_job.data<Function*>();

            try {
                (*function)(the_job);
                delete function;
            }
            catch(...) {
                delete function;
                throw;
            }
        };

        new(j) job(job_fn, parent);
        Function* dynamic_alloc = new Function{fn};
        j->set_data(dynamic_alloc);
    }
};

}

#endif //SPOOKYSUSHI_JOB_HPP
