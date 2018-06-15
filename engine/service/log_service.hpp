#ifndef SPOOKYSUSHI_LOG_SERVICE_HPP
#define SPOOKYSUSHI_LOG_SERVICE_HPP

#include "../debug/logger.hpp"
#include "../core/string_utils.hpp"

namespace sushi {

class log_service {
    static debug::logger* location;
public:
    static void locate(debug::logger* engine);
    static debug::logger& get() noexcept;
};

template<typename... Args>
void log_critical(const char* category, const char* format, Args... args) noexcept {
    log_service::get().log_critical(category, formatted_string(format, args...));
}

template<typename... Args>
void log_warning(const char* category, const char* format, Args... args) noexcept {
    log_service::get().log_warning(category, formatted_string(format, args...));
}

template<typename... Args>
void log_info(const char* category, const char* format, Args... args) noexcept {
    log_service::get().log_info(category, formatted_string(format, args...));
}

}

#endif //SPOOKYSUSHI_LOG_SERVICE_HPP
