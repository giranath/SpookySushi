#ifndef SPOOKYSUSHI_LOGGER_HPP
#define SPOOKYSUSHI_LOGGER_HPP

#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

namespace sushi { namespace debug {

enum class log_priority {
    critical,
    warning,
    info
};

std::ostream& operator<<(std::ostream& os, log_priority priority) noexcept;

class log_entry {
    std::chrono::system_clock::time_point time_point_;
    std::string message_;
    std::string category_;
    log_priority priority_;
    std::thread::id thread_id_;
public:
    log_entry(const std::string& category, log_priority priority, const std::string& message, std::thread::id tid) noexcept;

    const std::string& message() const noexcept;
    const std::string& category() const noexcept;
    log_priority priority() const noexcept;
    std::chrono::system_clock::time_point time_point() const noexcept;
    std::thread::id thread_id() const noexcept;
};

std::ostream& operator<<(std::ostream& os, const log_entry& entry) noexcept;

class logger {
    std::vector<log_entry> entries;
    std::mutex entries_mutex;
    std::thread background_thread;
    std::atomic_bool is_running;

    void background_work();
public:
    logger() noexcept;
    ~logger() noexcept;

    void start() noexcept;
    void stop() noexcept;
    void log(const log_entry& entry);

    void log_warning(const std::string& category, const std::string& message) noexcept;
    void log_critical(const std::string& category, const std::string& message) noexcept;
    void log_info(const std::string& category, const std::string& message) noexcept;
};

}}

#endif //SPOOKYSUSHI_LOGGER_HPP
