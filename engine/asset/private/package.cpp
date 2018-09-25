#include "package.hpp"
#include <murmurhash.hpp>
#include <compress.hpp>

#include <cstring>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <cassert>

namespace sushi {

std::istream& operator>>(std::istream& stream, PackageHeader& header) {
    char magic[4];
    stream.read(magic, sizeof(magic));
    for(int i = 0; i < 4; ++i) {
        if(magic[i] != PackageHeader::Magic[i]) {
            stream.setstate(std::ios::failbit);
            return stream;
        }
    }

    stream.read(reinterpret_cast<char*>(&header.flags), 2);
    stream.read(reinterpret_cast<char*>(&header.version), 1);
    stream.ignore(1);

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const PackageHeader& header) {
    stream.write(PackageHeader::Magic, sizeof(char) * 4);
    stream.write(reinterpret_cast<const char*>(&header.flags), 2);
    stream.write(reinterpret_cast<const char*>(&header.version), 1);

    uint8_t ignored_byte = 0xFF;
    stream.write(reinterpret_cast<const char*>(&ignored_byte), 1);

    return stream;
}

std::istream& operator>>(std::istream& stream, PackageEntry& entry) {
    stream.read(reinterpret_cast<char*>(&entry.identifier), 2);
    stream.read(reinterpret_cast<char*>(&entry.type), 2);
    stream.read(reinterpret_cast<char*>(&entry.offset), 8);
    stream.read(reinterpret_cast<char*>(&entry.size), 8);
    stream.read(reinterpret_cast<char*>(&entry.checksum), 4);
    stream.ignore(8);

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const PackageEntry& entry) {
    stream.write(reinterpret_cast<const char*>(&entry.identifier), 2);
    stream.write(reinterpret_cast<const char*>(&entry.type), 2);
    stream.write(reinterpret_cast<const char*>(&entry.offset), 8);
    stream.write(reinterpret_cast<const char*>(&entry.size), 8);
    stream.write(reinterpret_cast<const char*>(&entry.checksum), 4);

    const uint64_t reserved_bytes = 0xffffffffffffffff;
    stream.write(reinterpret_cast<const char*>(&reserved_bytes), 8);

    return stream;
}

Package::Package()
: Package{PackageHeader{}}{

}

Package::Package(PackageHeader header)
: header{header}
, entries{}
, data{}
, is_valid_{true} {

}

bool Package::is_valid() const noexcept {
    return is_valid_;
}

void Package::add_entry(PackageEntry entry, const std::vector<uint8_t>& data) {
    entries.push_back(entry);
    std::copy(std::begin(data), std::end(data), std::back_inserter(this->data));
}

std::optional<std::string_view> Package::entry_data(PackageEntry::Identifier identifier) const noexcept {
    auto it = std::find_if(std::begin(entries), std::end(entries), [identifier](const PackageEntry& entry) {
        return entry.identifier == identifier;
    });

    if(it != std::end(entries)) {
        return std::string_view(reinterpret_cast<const char*>(data.data() + it->offset), it->size);
    }

    return {};
}

std::optional<std::string_view> Package::entry_data(PackageEntry::Identifier identifier, ValidateEntry) const noexcept {
    auto it = std::find_if(std::begin(entries), std::end(entries), [identifier](const PackageEntry& entry) {
        return entry.identifier == identifier;
    });

    if(it != std::end(entries)) {
        const std::string_view data_view(reinterpret_cast<const char*>(data.data() + it->offset), it->size);

        const uint32_t checksum = murmur_hash3_x86_32(data_view.data(), data_view.size(), 0);

        if(checksum == it->checksum) {
            return data_view;
        }
    }

    return {};
}

bool Package::is_entry_valid(PackageEntry::Identifier identifier) const noexcept {
    auto it = std::find_if(std::begin(entries), std::end(entries), [identifier](const PackageEntry& entry) {
        return entry.identifier == identifier;
    });

    if(it != std::end(entries)) {
        const uint32_t checksum = murmur_hash3_x86_32(reinterpret_cast<const char*>(data.data()) + it->offset, it->size, 0);

        return checksum == it->checksum;
    }

    return false;
}

std::istream& operator>>(std::istream& stream, Package& package) {
    package.is_valid_ = false;

    PackageHeader header;
    if(stream >> header) {
        package.header = header;

        uint16_t nb_entries;
        uint16_t data_offset;
        stream.read(reinterpret_cast<char*>(&nb_entries), 2);
        stream.read(reinterpret_cast<char*>(&data_offset), 2);
        stream.ignore(4);

        // Read all the entries
        for(uint16_t entry_id = 0; entry_id < nb_entries; ++entry_id) {
            PackageEntry entry;
            if(stream >> entry) {
                package.entries.push_back(entry);
            }
            else {
                return stream;
            }
        }

        // When there is no compression involved
        const std::size_t total_size = std::accumulate(std::begin(package.entries),
                                                       std::end(package.entries),
                                                       static_cast<std::size_t>(0),
                                                       [](std::size_t i, const PackageEntry& entry) {
                                                           return i + entry.size;
                                                       });

        if((package.header.flags & PackageHeader::MaskCompressed) == 0) {
            for(std::size_t i = 0; i < total_size && stream; ++i) {
                uint8_t byte;
                if(stream.read(reinterpret_cast<char*>(&byte), 1)) {
                    package.data.push_back(byte);
                }
            }
        }
        else {
            uint64_t compressed_length;
            stream.read(reinterpret_cast<char*>(&compressed_length), sizeof(uint64_t));

            std::vector<uint8_t> bytes;
            bytes.reserve(compressed_length);
            for(std::size_t i = 0; i < compressed_length; ++i) {
                uint8_t byte;
                if(stream.read(reinterpret_cast<char*>(&byte), 1)) {
                    bytes.push_back(byte);
                }
            }

            package.data.reserve(total_size);
            sushi::decompress(std::begin(bytes), std::end(bytes),
                              std::back_inserter(package.data), total_size);

            if(package.data.size() != total_size) {
                stream.setstate(std::ios::failbit);
            }
        }

        if(stream.good()) {
            package.is_valid_ = true;
        }
    }

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Package& package) {
    if(stream << package.header) {
        const std::size_t entries_count = package.entries.size();
        if(entries_count >= std::numeric_limits<uint16_t>::max()) {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        const uint16_t nb_entries = static_cast<uint16_t>(entries_count);
        stream.write(reinterpret_cast<const char*>(&nb_entries), 2);

        const std::size_t data_offset_large = entries_count * 32; // 32 is the size of an entry
        if(data_offset_large >= std::numeric_limits<uint16_t>::max()) {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        const uint16_t data_offset = static_cast<uint16_t>(data_offset_large);
        stream.write(reinterpret_cast<const char*>(&data_offset), 2);

        const uint32_t reserved_bytes = 0xffffffff;
        stream.write(reinterpret_cast<const char*>(&reserved_bytes), 4);

        // Write the entries
        if(stream) {
            std::copy(std::begin(package.entries), std::end(package.entries),
                      std::ostream_iterator<PackageEntry>(stream));
        }

        // Write all the data stored in the package
        if(stream) {
            if((package.header.flags & PackageHeader::MaskCompressed) != 0) {
                std::vector<uint8_t> compressed_bytes;
                compressed_bytes.reserve(package.data.size());
                sushi::compress(std::begin(package.data), std::end(package.data),
                                std::back_inserter(compressed_bytes));

                const uint64_t compressed_length = compressed_bytes.size();
                stream.write(reinterpret_cast<const char*>(&compressed_length), sizeof(uint64_t));
                std::copy(std::begin(compressed_bytes), std::end(compressed_bytes),
                          std::ostreambuf_iterator<char>(stream));
            }
            else {
                // Do not compress the package
                std::copy(std::begin(package.data), std::end(package.data),
                          std::ostreambuf_iterator<char>(stream));
            }
        }
    }

    return stream;
}

}