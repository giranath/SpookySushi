#include "log_service.hpp"

namespace sushi {

debug::logger* LogService::location = nullptr;

void LogService::locate(debug::logger* logger) {
    location = logger;
}

debug::logger& LogService::get() noexcept {
    return *location;
}

}