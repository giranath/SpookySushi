#ifndef SPOOKYSUSHI_INPUT_BUS_SERVICE_HPP
#define SPOOKYSUSHI_INPUT_BUS_SERVICE_HPP

#include "service_locator.hpp"

namespace sushi {

class InputBusReader;

class InputBusService : public ServiceLocator<sushi::InputBusReader> {};

}

#endif //SPOOKYSUSHI_JOB_SERVICE_HPP
