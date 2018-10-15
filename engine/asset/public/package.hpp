#ifndef SPOOKYSUSHI_PACKAGE_HPP
#define SPOOKYSUSHI_PACKAGE_HPP

#include "package_types.hpp"
#include <cstdint>
#include <vector>
#include <iostream>
#include <string_view>
#include <optional>

namespace sushi {

struct PackageHeader {
    static constexpr char Magic[4] = {'S', 'A', 'P', 'K'};
    enum {
        MaskLittleEndian = 1,
        MaskCompressed = (1 << 1)
    };

    uint16_t flags {};
    uint8_t version {};

    PackageHeader() = default;
    PackageHeader(uint8_t version, uint16_t flags)
    : flags{flags}
    , version{version} {

    }
};

std::istream& operator>>(std::istream& stream, PackageHeader& header);
std::ostream& operator<<(std::ostream& stream, const PackageHeader& header);

struct PackageEntry {
    using Identifier = uint16_t;
    Identifier identifier;
    PackageType type;
    uint64_t offset;
    uint64_t size;
    uint32_t checksum;
    uint64_t reserved;
};

std::istream& operator>>(std::istream& stream, PackageEntry& entry);
std::ostream& operator<<(std::ostream& stream, const PackageEntry& entry);

struct ValidateEntry{};

// TODO: Let packages stream their content

// A package hold
class Package {
    PackageHeader header;
    std::vector<PackageEntry> entries;
    std::vector<uint8_t> data;
    bool is_valid_;
public:
    Package();
    explicit Package(PackageHeader header);

    bool is_valid() const noexcept;

    void add_entry(PackageEntry entry, const std::vector<uint8_t>& data);

    // TODO: Return a stream
    std::optional<std::string_view> entry_data(PackageEntry::Identifier identifier) const noexcept;
    std::optional<std::string_view> entry_data(PackageEntry::Identifier identifier, ValidateEntry) const noexcept;
    bool is_entry_valid(PackageEntry::Identifier identifier) const noexcept;

    friend std::istream& operator>>(std::istream& stream, Package& package);
    friend std::ostream& operator<<(std::ostream& stream, const Package& package);
};



}

#endif //SPOOKYSUSHI_PACKAGE_HPP
