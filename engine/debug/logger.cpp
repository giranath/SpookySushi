#include "logger.hpp"

#include <iterator>
#include <algorithm>

namespace sushi { namespace debug {

std::ostream& operator<<(std::ostream& os, log_priority priority) noexcept {
    switch(priority) {
        case log_priority::critical:
            os << "crit";
            break;
        case log_priority::info:
            os << "info";
            break;
        case log_priority::warning:
            os << "warn";
            break;
    }
    return os;
}

log_entry::log_entry(const std::string& category, log_priority priority, const std::string& message, std::thread::id tid) noexcept
: time_point_(std::chrono::system_clock::now())
, message_(message)
, category_(category)
, priority_{priority}
, thread_id_{tid} {

}

const std::string& log_entry::message() const noexcept {
    return message_;
}

const std::string& log_entry::category() const noexcept {
    return category_;
}

log_priority log_entry::priority() const noexcept {
    return priority_;
}

std::chrono::system_clock::time_point log_entry::time_point() const noexcept {
    return time_point_;
}

std::thread::id log_entry::thread_id() const noexcept {
    return thread_id_;
}

std::ostream& operator<<(std::ostream& os, const log_entry& entry) noexcept {
    char time_buffer[128];
    std::fill(time_buffer, time_buffer + 128, 0);

    const std::time_t t = std::chrono::system_clock::to_time_t(entry.time_point());
    strftime(time_buffer, 128, "[%H:%M:%S %z]", std::localtime(&t));

    return os << entry.priority() << " "
              << entry.category() << " "
              << entry.thread_id() << " "
              << time_buffer
              << " \"" << entry.message() << "\"";
}

logger::logger() noexcept
: entries{}
, entries_mutex{}
, background_thread{}
, is_running{false} {
    entries.reserve(2048);
}

logger::~logger() noexcept {
    stop();
}

void logger::start() noexcept {
    is_running = true;
    background_thread = std::thread(&logger::background_work, std::ref(*this));
}

void logger::stop() noexcept {
    is_running.store(false);

    if(background_thread.joinable()) {
        background_thread.join();
    }
}

void logger::log(const log_entry& entry) {
    std::lock_guard<std::mutex> lock(entries_mutex);

    entries.push_back(entry);
}

void logger::log_warning(const std::string& category, const std::string& message) noexcept {
    std::lock_guard<std::mutex> lock(entries_mutex);

    entries.emplace_back(category, log_priority::warning, message, std::this_thread::get_id());
}

void logger::log_critical(const std::string& category, const std::string& message) noexcept {
    std::lock_guard<std::mutex> lock(entries_mutex);

    entries.emplace_back(category, log_priority::critical, message, std::this_thread::get_id());
}

void logger::log_info(const std::string& category, const std::string& message) noexcept {
    std::lock_guard<std::mutex> lock(entries_mutex);

    entries.emplace_back(category, log_priority::info, message, std::this_thread::get_id());
}

void logger::background_work() {
    std::ofstream out_log("latest_session.log", std::ios::binary);

    std::vector<log_entry> write_entries;
    write_entries.reserve(2048);

    while(is_running) {
        {
            std::lock_guard<std::mutex> lock(entries_mutex);
            entries.swap(write_entries);
        }

        // Dump all write entries
        std::copy(std::begin(write_entries), std::end(write_entries), std::ostream_iterator<log_entry>(out_log));
        write_entries.clear();
        out_log.flush();

        std::this_thread::yield();
    }

    // Dump every entries
    std::copy(std::begin(entries), std::end(entries), std::ostream_iterator<log_entry>(out_log));
}

}}