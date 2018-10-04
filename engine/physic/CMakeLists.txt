add_library(sushi-physic
        public/physic_world.hpp
        private/physic_world.cpp)

target_link_libraries(sushi-physic
        PUBLIC sushi-core
        PUBLIC sushi-service
        PRIVATE libBullet3Common
        PRIVATE libBullet3Dynamics
        PRIVATE libBullet3Collision
        PRIVATE libBulletDynamics
        PRIVATE libBulletCollision
        PRIVATE libBulletLinearMath)
target_include_directories(sushi-physic
        PUBLIC public
        PRIVATE private)