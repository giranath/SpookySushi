add_library(sushi-physic
        public/physic_world_traits.hpp
        private/physic_world_traits.cpp
        public/rigid_body_traits.hpp
        private/bullet_debug_drawer.cpp
        private/bullet_debug_drawer.hpp
        private/bullet_rigid_body.cpp
        public/physic_transform.hpp
        public/bullet_physic_world.hpp
        public/physic_world.hpp
        private/bullet_physic_world.cpp
        public/bullet_rigid_body.hpp
        public/physic_shape.hpp)

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