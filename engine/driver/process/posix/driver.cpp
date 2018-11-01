#include "../process.hpp"

#include <unistd.h>

namespace sushi { namespace driver { namespace process {

process_id current_process_id() noexcept {
    return static_cast<process_id >(::getpid());
}

}}}