#ifndef SPOOKYSUSHI_PROXY_RENDERER_HPP
#define SPOOKYSUSHI_PROXY_RENDERER_HPP

#include "renderer_interface.hpp"
#include "mesh.hpp"

namespace sushi {

class ProxyRenderer {
    RendererInterface* renderer;
public:
    explicit ProxyRenderer(RendererInterface* renderer) noexcept;
};

}

#endif
