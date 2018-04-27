#ifndef SPOOKYSUSHI_JOB_SERVICE_HPP
#define SPOOKYSUSHI_JOB_SERVICE_HPP

#include "../async/engine.hpp"

namespace sushi {

class jobs_service {
    static async::engine* location;
public:
    static void locate(async::engine* engine);
    static async::engine& get() noexcept;
};

}

#endif //SPOOKYSUSHI_JOB_SERVICE_HPP
