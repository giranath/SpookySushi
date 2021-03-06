#ifndef SPOOKYSUSHI_LOG_SERVICE_HPP
#define SPOOKYSUSHI_LOG_SERVICE_HPP

#include "service_locator.hpp"
#include "logger.hpp"
#include "string_utils.hpp"

namespace sushi {

class LogService : public ServiceLocator<debug::Logger> {};

template<typename... Args>
void log_critical(const char* category, const char* format, Args... args) noexcept {
    LogService::get().log_critical(category, formatted_string(format, args...));
}

template<typename... Args>
void log_warning(const char* category, const char* format, Args... args) noexcept {
    LogService::get().log_warning(category, formatted_string(format, args...));
}

template<typename... Args>
void log_info(const char* category, const char* format, Args... args) noexcept {
    LogService::get().log_info(category, formatted_string(format, args...));
}

}

#endif //SPOOKYSUSHI_LOG_SERVICE_HPP
