#include "job_service.hpp"

namespace sushi {

async::engine* jobs_service::location = nullptr;

void jobs_service::locate(async::engine* engine) {
    location = engine;
}

async::engine& jobs_service::get() noexcept {
    return *location;
}

}