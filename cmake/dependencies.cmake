include(ExternalProject)

set(DEPENDENCIES_ROOT "${CMAKE_BINARY_DIR}/dependencies")

#=======================================================================================================================
# SDL2
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/SDL2)
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/SDL2/include)
ExternalProject_Add(SDL2
        URL "https://www.libsdl.org/release/SDL2-2.0.8.zip"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/SDL2"
        INSTALL_DIR "${DEPENDENCIES_ROOT}/SDL2")

# Create a target to reference the external project
add_library(libSDL2 IMPORTED SHARED GLOBAL)
add_library(libSDL2main IMPORTED STATIC GLOBAL)
add_dependencies(libSDL2 SDL2)
add_dependencies(libSDL2main SDL2)

if(MSVC)
    set_target_properties(libSDL2 PROPERTIES
            IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX}"
            IMPORTED_IMPLIB "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX}"
            IMPORTED_LOCATION_DEBUG "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2d${CMAKE_SHARED_LIBRARY_SUFFIX}"
            IMPORTED_IMPLIB_DEBUG "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2d${CMAKE_STATIC_LIBRARY_SUFFIX}"
            INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/SDL2/include/SDL2")
    set_target_properties(libSDL2main PROPERTIES
            IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}"
            IMPORTED_LOCATION_DEBUG "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}"
            INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/SDL2/include/SDL2")
else()
    set_target_properties(libSDL2 PROPERTIES
            IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX}"
            INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/SDL2/include/SDL2")
    set_target_properties(libSDL2main PROPERTIES
            IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}"
            INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/SDL2/include/SDL2")
endif()

#=======================================================================================================================
# glm
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/glm)
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/glm/include)
ExternalProject_Add(glm
    URL https://github.com/g-truc/glm/archive/0.9.9.0.zip
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/glm"
    INSTALL_DIR "${DEPENDENCIES_ROOT}/glm")
add_library(libGlm INTERFACE IMPORTED GLOBAL)
add_dependencies(libGlm glm)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(GLM_COMPILE_DEFINITIONS "GLM_FORCE_CTOR_INIT=1")
else()
    set(GLM_COMPILE_DEFINITIONS "")
endif()

set_target_properties(libGlm PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/glm/include"
        INTERFACE_COMPILE_DEFINITIONS "${GLM_COMPILE_DEFINITIONS}")

#=======================================================================================================================
# glad
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/glad)
ExternalProject_Add(glad
        URL https://github.com/Dav1dde/glad/archive/v0.1.24.zip

        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/glad -DGLAD_PROFILE=core"
        INSTALL_DIR "${DEPENDENCIES_ROOT}/glad"
        INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory include "${DEPENDENCIES_ROOT}/glad/include"
                        COMMAND "${CMAKE_COMMAND}" -E copy_directory src "${DEPENDENCIES_ROOT}/glad/src")
add_library(libGlad INTERFACE IMPORTED GLOBAL)
add_dependencies(libGlad glad)

set_target_properties(libGlad PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/glad/include"
        INTERFACE_SOURCES "${DEPENDENCIES_ROOT}/glad/src/glad.c"
        INTERFACE_LINK_LIBRARIES "${CMAKE_DL_LIBS}")

#=======================================================================================================================
# Bullet Physic
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/bullet3)
ExternalProject_Add(Bullet3
        URL https://github.com/bulletphysics/bullet3/archive/2.87.zip

        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/bullet3" -DBUILD_CPU_DEMOS=OFF -DBUILD_PYBULLET=OFF -DBUILD_ENET=OFF -DBUILD_CLSOCKET=OFF -DBUILD_OPENGL3_DEMOS=OFF -DBUILD_BULLET2_DEMOS=OFF -DBUILD_EXTRAS=OFF -DBUILD_UNIT_TESTS=OFF -DINSTALL_CMAKE_FILES=OFF -DUSE_GRAPHICAL_BENCHMARK=OFF
        INSTALL_DIR "${DEPENDENCIES_ROOT}/bullet3")

add_library(libBullet3Common IMPORTED STATIC GLOBAL)
add_library(libBullet3Collision IMPORTED STATIC GLOBAL)
add_library(libBullet3Dynamics IMPORTED STATIC GLOBAL)
add_library(libBulletCollision IMPORTED STATIC GLOBAL)
add_library(libBulletDynamics IMPORTED STATIC GLOBAL)
add_library(libBulletLinearMath IMPORTED STATIC GLOBAL)
add_dependencies(libBullet3Common Bullet3)
add_dependencies(libBullet3Collision Bullet3)
add_dependencies(libBullet3Dynamics Bullet3)
add_dependencies(libBulletCollision Bullet3)
add_dependencies(libBulletDynamics Bullet3)
add_dependencies(libBulletLinearMath Bullet3)

set_target_properties(libBullet3Common PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/bullet3/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Bullet3Common${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/bullet3/include/bullet")
set_target_properties(libBullet3Collision PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/bullet3/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Bullet3Collision${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/bullet3/include/bullet")
set_target_properties(libBullet3Dynamics PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/bullet3/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Bullet3Dynamics${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/bullet3/include/bullet")
set_target_properties(libBulletCollision PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/bullet3/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletCollision${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/bullet3/include/bullet")
set_target_properties(libBulletDynamics PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/bullet3/lib/${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/bullet3/include/bullet")
set_target_properties(libBulletLinearMath PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/bullet3/lib/${CMAKE_STATIC_LIBRARY_PREFIX}LinearMath${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/bullet3/include/bullet")

#=======================================================================================================================
# Rapidxml
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/rapidxml)
ExternalProject_Add(rapidxml
        URL https://downloads.sourceforge.net/project/rapidxml/rapidxml/rapidxml%201.13/rapidxml-1.13.zip?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Frapidxml%2Ffiles%2Flatest%2Fdownload&ts=1531707023
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        CMAKE_ARGS ""
        INSTALL_DIR "${DEPENDENCIES_ROOT}/rapidxml"
        INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy
            ../rapidxml/rapidxml.hpp
            ../rapidxml/rapidxml_iterators.hpp
            ../rapidxml/rapidxml_print.hpp
            ../rapidxml/rapidxml_utils.hpp
            ${DEPENDENCIES_ROOT}/rapidxml/)
add_library(libRapidxml INTERFACE IMPORTED GLOBAL)
add_dependencies(libRapidxml rapidxml)

set_target_properties(libRapidxml PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/rapidxml/")

#=======================================================================================================================
# Assimp
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/assimp)
ExternalProject_Add(assimp
        URL https://github.com/assimp/assimp/archive/v4.1.0.zip

        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/assimp -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DINJECT_DEBUG_POSTFIX=OFF)
add_library(libAssimp IMPORTED SHARED GLOBAL)
add_dependencies(libAssimp assimp)
set_target_properties(libAssimp PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/assimp/lib/${CMAKE_SHARED_LIBRARY_PREFIX}assimp${CMAKE_SHARED_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/assimp/include")

#=======================================================================================================================
# Zlib
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/zlib)
ExternalProject_Add(zlib
        URL https://zlib.net/zlib1211.zip

        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/zlib)
add_library(libZlib IMPORTED STATIC GLOBAL)
add_dependencies(libZlib zlib)
set_target_properties(libZlib PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/zlib/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/zlib/include")