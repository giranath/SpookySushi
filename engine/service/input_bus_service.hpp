#ifndef SPOOKYSUSHI_INPUT_BUS_SERVICE_HPP
#define SPOOKYSUSHI_INPUT_BUS_SERVICE_HPP

#include "service_locator.hpp"

namespace sushi {

class InputBusReader;
class InputProcessor;

class InputBusService : public ServiceLocator<sushi::InputBusReader> {};
class InputProcessorService : public ServiceLocator<sushi::InputProcessor> {};

}

#endif //SPOOKYSUSHI_JOB_SERVICE_HPP
