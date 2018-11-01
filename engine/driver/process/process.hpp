#ifndef SPOOKYSUSHI_PROCESS_HPP
#define SPOOKYSUSHI_PROCESS_HPP

#include <cstdint>

namespace sushi { namespace driver { namespace process {

using process_id = uint32_t;

process_id current_process_id() noexcept;

}}}

#endif //SPOOKYSUSHI_PROCESS_HPP
