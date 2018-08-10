#ifndef SPOOKYSUSHI_PACKAGE_HPP
#define SPOOKYSUSHI_PACKAGE_HPP

#include "package_types.hpp"
#include <cstdint>
#include <vector>
#include <iostream>

namespace sushi {

struct PackageHeader {
    static constexpr char Magic[4] = {'S', 'A', 'P', 'K'};
    enum {
        MaskLittleEndian = 0x1
    };

    uint16_t flags {};
    uint8_t version {};

    PackageHeader() = default;

    static std::istream& loadFromStream(std::istream& input_stream, PackageHeader& package);
    static std::ostream& saveToStream(std::ostream& output_stream, const PackageHeader& package);
};

struct PackageEntry {
    uint16_t identifier;
    PackageType type;
    uint64_t offset;
    uint64_t size;
    uint32_t checksum;
    uint64_t reserved;
};

// A package hold
class Package {
    std::vector<PackageEntry> entries;
    uint8_t* content;
    bool is_valid_;
public:
    bool is_valid() const noexcept;

    static std::istream& loadFromStream(std::istream& input_stream, Package& package);
    static std::ostream& saveToStream(std::ostream& output_stream, const Package& package);
};

}

#endif //SPOOKYSUSHI_PACKAGE_HPP
