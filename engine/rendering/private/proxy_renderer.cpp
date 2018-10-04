#include "proxy_renderer.hpp"

namespace sushi {

ProxyRenderer::ProxyRenderer(RendererInterface* renderer) noexcept
: renderer(renderer) {

}

void ProxyRenderer::set_active_camera(Camera* camera) {
    renderer->set_active_camera(camera);
}

}