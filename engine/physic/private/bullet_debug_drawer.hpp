#ifndef SPOOKYSUSHI_BULLET_DEBUG_DRAWER_HPP
#define SPOOKYSUSHI_BULLET_DEBUG_DRAWER_HPP

#include <btBulletDynamicsCommon.h>

namespace sushi {

class BulletDebugDrawer : public btIDebugDraw {
    int debug_mode;
public:
    BulletDebugDrawer();

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

    void drawSphere (const btVector3& p, btScalar radius, const btVector3& color) override;

    void drawAabb(const btVector3& from, const btVector3& to,const btVector3& color) override;

    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;

    void reportErrorWarning(const char* warningString) override;

    void draw3dText(const btVector3& location, const char* textString) override;

    void setDebugMode(int debugMode) override;

    int getDebugMode() const override;
};

}


#endif //SPOOKYSUSHI_BULLET_DEBUG_DRAWER_HPP
