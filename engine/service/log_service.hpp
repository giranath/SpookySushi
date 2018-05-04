#ifndef SPOOKYSUSHI_LOG_SERVICE_HPP
#define SPOOKYSUSHI_LOG_SERVICE_HPP

#include "../debug/logger.hpp"

namespace sushi {

class log_service {
    static debug::logger* location;
public:
    static void locate(debug::logger* engine);
    static debug::logger& get() noexcept;
};

}

#endif //SPOOKYSUSHI_LOG_SERVICE_HPP
