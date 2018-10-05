#include "bullet_debug_drawer.hpp"

#include <btBulletDynamicsCommon.h>
#include <debug_draw_service.hpp>
#include <renderer_interface.hpp>
#include <log_service.hpp>

namespace sushi {

Vec3 to_vec3(const btVector3& bt_vector3) noexcept {
    return Vec3{bt_vector3.x(), bt_vector3.y(), bt_vector3.z()};
}

RGBColor to_rgb(const btVector3& bt_color) noexcept {
    return RGBColor(bt_color.x(), bt_color.y(), bt_color.z());
}


BulletDebugDrawer::BulletDebugDrawer()
: debug_mode{} {

}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    DebugRendererService::get().add_line(to_vec3(from), to_vec3(to), to_rgb(color));
}

void BulletDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color) {
    DebugRendererService::get().add_sphere(to_vec3(p), radius, to_rgb(color));
}

void BulletDebugDrawer::drawAabb(const btVector3& from, const btVector3& to,const btVector3& color) {
    const float width = std::abs(to.x() - from.x());
    const float height = std::abs(to.y() - from.y());
    const float depth = std::abs(to.z() - from.z());

    const float half_width = width / 2.f;
    const float half_height = height / 2.f;
    const float half_depth = depth / 2.f;

    const Vec3 extend{half_width, half_height, half_depth};

    DebugRendererService::get().add_aabb(to_vec3(from) + extend, extend, to_rgb(color));
}

void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {

}

void BulletDebugDrawer::reportErrorWarning(const char* warningString) {
    log_warning("sushi.physic.bullet3", warningString);
}

void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString) {
    // TODO: Draw 3d text
}

void BulletDebugDrawer::setDebugMode(int debugMode) {
    debug_mode = debugMode;
}

int BulletDebugDrawer::getDebugMode() const {
    return debug_mode;
}

}