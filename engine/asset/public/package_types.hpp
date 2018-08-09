#ifndef SPOOKYSUSHI_PACKAGE_TYPES_HPP
#define SPOOKYSUSHI_PACKAGE_TYPES_HPP

#include <cstdint>

namespace sushi {
    /**
     * Default supported package types
     */
    enum PackageType : uint16_t {
        PACKAGE_3D_MODEL,
        PACKAGE_UNKNOWN
    };
}

#endif
