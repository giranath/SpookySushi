#include "log_service.hpp"

namespace sushi {

debug::logger* log_service::location = nullptr;

void log_service::locate(debug::logger* logger) {
    location = logger;
}

debug::logger& log_service::get() noexcept {
    return *location;
}

}