#ifndef SPOOKYSUSHI_PACKAGE_HPP
#define SPOOKYSUSHI_PACKAGE_HPP

#include "package_types.hpp"
#include <cstdint>

namespace sushi {

class PackageHeader {
    // A package magic
    static constexpr char Magic[4] = {'S', 'A', 'P', 'K'};
    enum {
        MaskLittleEndian = 0x1
    };

    uint16_t flags;
    uint8_t version;
};

class PackageEntryTable {

};

// A package hold
class Package {
    PackageHeader header;
    PackageEntryTable entries;
    uint8_t* content;
};

}

#endif //SPOOKYSUSHI_PACKAGE_HPP
