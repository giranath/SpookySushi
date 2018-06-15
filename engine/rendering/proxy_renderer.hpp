#ifndef SPOOKYSUSHI_PROXY_RENDERER_HPP
#define SPOOKYSUSHI_PROXY_RENDERER_HPP

#include "renderer_interface.hpp"

namespace sushi {

class proxy_renderer {
    renderer_interface* renderer;
public:
    explicit proxy_renderer(renderer_interface* renderer) noexcept;
};

}

#endif
