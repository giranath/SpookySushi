#include "process.hpp"

#include <Windows.h>

namespace sushi { namespace driver { namespace process {

process_id current_process_id() noexcept {
    return static_cast<process_id >(::GetCurrentProcessId());
}


}}}