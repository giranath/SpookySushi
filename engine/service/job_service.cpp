#include "job_service.hpp"

namespace sushi {

async::engine* JobsService::location = nullptr;

void JobsService::locate(async::engine* engine) {
    location = engine;
}

async::engine& JobsService::get() noexcept {
    return *location;
}

}