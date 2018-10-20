if("${SUSHI_PHYSIC_BACKEND}" STREQUAL "Bullet")
    set(SUSHI_PHYSIC_BACKEND_BULLET ON)
elseif("${SUSHI_PHYSIC_BACKEND}" STREQUAL "PhysX")
    set(SUSHI_PHYSIC_BACKEND_PHYSX ON)
endif()

configure_file(physic_backend.in.hpp ${CMAKE_CURRENT_SOURCE_DIR}/public/physic_backend.hpp)

add_library(sushi-physic
        public/physic_world_traits.hpp
        private/physic_world_traits.cpp
        public/rigid_body_traits.hpp
        public/physic_transform.hpp
        public/physic_world.hpp
        public/physic_shape.hpp)

if("${SUSHI_PHYSIC_BACKEND}" STREQUAL "Bullet")
    target_sources(sushi-physic PRIVATE
            private/bullet/bullet_debug_drawer.cpp
            private/bullet/bullet_debug_drawer.hpp
            private/bullet/bullet_rigid_body.cpp
            public/bullet/bullet_physic_world.hpp
            private/bullet/bullet_physic_world.cpp
            public/bullet/bullet_rigid_body.hpp)
    target_include_directories(sushi-physic
            PUBLIC public/bullet
            PRIVATE private/bullet)
elseif("${SUSHI_PHYSIC_BACKEND}" STREQUAL "PhysX")
    target_sources(sushi-physic PRIVATE
            public/physx/physx_physic_world.hpp
            public/physx/physx_rigid_body.hpp
            private/physx/physx_physic_world.cpp
            private/physx/physx_foundation.cpp
            private/physx/physx_foundation.hpp
            private/physx/physx_physics.h
            private/physx/physx_physics.cpp)
    target_include_directories(sushi-physic
            PUBLIC public/physx
            PRIVATE private/physx)
endif()

target_link_libraries(sushi-physic
        PUBLIC sushi-core
        PRIVATE sushi-service
        PRIVATE PhysicBackend )
target_include_directories(sushi-physic
        PUBLIC public
        PRIVATE private)