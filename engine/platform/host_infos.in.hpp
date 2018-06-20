#ifndef SPOOKYSUSHI_HOST_INFOS_HPP
#define SPOOKYSUSHI_HOST_INFOS_HPP

#include <cstddef>

#cmakedefine01 SUSHI_USE_OPENGL
#cmakedefine SUSHI_CACHE_LINE_SIZE

namespace sushi {

constexpr const std::size_t CACHE_LINE_SIZE = ${SUSHI_CACHE_LINE_SIZE};
constexpr const char* SYSTEM_NAME = "${CMAKE_SYSTEM_NAME}";
constexpr const char* SYSTEM_VERSION = "${CMAKE_SYSTEM_VERSION}";
constexpr const char* SYSTEM_FULL_NAME = "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}";

}

#endif