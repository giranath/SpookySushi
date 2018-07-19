#include "job.hpp"

namespace sushi { namespace async {

void job::terminate() noexcept {
    --unfinished_count;

    if(parent && is_finished()) {
        parent->terminate();
    }
}

void job::execute() {
    // Execute the handling function
    if(fn) {
        fn(*this);
    }

    terminate();
}

bool job::is_finished() const noexcept {
    return unfinished_count == 0;
}

}}