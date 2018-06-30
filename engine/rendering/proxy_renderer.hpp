#ifndef SPOOKYSUSHI_PROXY_RENDERER_HPP
#define SPOOKYSUSHI_PROXY_RENDERER_HPP

#include "renderer_interface.hpp"
#include "mesh.hpp"

namespace sushi {

class ProxyRenderer {
    renderer_interface* renderer;
public:
    explicit ProxyRenderer(renderer_interface* renderer) noexcept;
};

}

#endif
