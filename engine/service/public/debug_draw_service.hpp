#ifndef SPOOKYSUSHI_DEBUG_DRAW_SERVICE_HPP
#define SPOOKYSUSHI_DEBUG_DRAW_SERVICE_HPP

#include "service_locator.hpp"

namespace sushi {

class DebugRendererInterface;

class DebugRendererService : public ServiceLocator<DebugRendererInterface> {};

}

#endif
