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

set_target_properties(libSDL2 PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/SDL2/include/SDL2")
set_target_properties(libSDL2main PROPERTIES
        IMPORTED_LOCATION "${DEPENDENCIES_ROOT}/SDL2/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}"
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/SDL2/include/SDL2")


#=======================================================================================================================
# TOML11
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/toml11)
ExternalProject_Add(toml11
        URL "https://github.com/ToruNiina/toml11/archive/v1.0.1.zip"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/toml11"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_DIR "${DEPENDENCIES_ROOT}/toml11"
        INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy ../toml11/toml.hpp "${DEPENDENCIES_ROOT}/toml11"
                        COMMAND "${CMAKE_COMMAND}" -E copy_directory ../toml11/toml "${DEPENDENCIES_ROOT}/toml11/toml")
add_library(libToml11 INTERFACE IMPORTED GLOBAL)
add_dependencies(libToml11 toml11)

set_target_properties(libToml11 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/toml11/")


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
# gl3w
#=======================================================================================================================
file(MAKE_DIRECTORY ${DEPENDENCIES_ROOT}/gl3w)
ExternalProject_Add(gl3w
        GIT_REPOSITORY https://github.com/skaslev/gl3w.git
        GIT_TAG 59f56cdecba45811bd269c93c252793f759d9deb # last version [july 7th 2018]

        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_ROOT}/gl3w"
        INSTALL_DIR "${DEPENDENCIES_ROOT}/gl3w")
add_library(libGl3w INTERFACE IMPORTED GLOBAL)
add_dependencies(libGl3w gl3w)

set_target_properties(libGl3w PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_ROOT}/gl3w/include/gl3w"
        INTERFACE_SOURCES "${DEPENDENCIES_ROOT}/gl3w/share/gl3w/gl3w.c"
        INTERFACE_LINK_LIBRARIES "${CMAKE_DL_LIBS}")