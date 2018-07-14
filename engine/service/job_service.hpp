#ifndef SPOOKYSUSHI_JOB_SERVICE_HPP
#define SPOOKYSUSHI_JOB_SERVICE_HPP

#include "ServiceLocator.hpp"
//#include "../async/engine.hpp"

namespace sushi {

namespace async {
class engine;
}

class JobsService : public ServiceLocator<async::engine> {

};

}

#endif //SPOOKYSUSHI_JOB_SERVICE_HPP
