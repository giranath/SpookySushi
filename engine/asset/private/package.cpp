#include "package.hpp"

#include <cstring>

namespace sushi {

std::istream& PackageHeader::loadFromStream(std::istream& input_stream, PackageHeader& package) {
    char magic[4];
    input_stream.read(magic, sizeof(magic));
    for(int i = 0; i < 4; ++i) {
        if(magic[i] != PackageHeader::Magic[i]) {
            input_stream.setstate(std::ios::failbit);
            return input_stream;
        }
    }

    input_stream.read(reinterpret_cast<char*>(package.flags), 2);
    input_stream.read(reinterpret_cast<char*>(package.version), 1);
    input_stream.ignore(1);

    return input_stream;
}

std::ostream& PackageHeader::saveToStream(std::ostream& output_stream, const PackageHeader& package) {
    return output_stream;
}

bool Package::is_valid() const noexcept {
    return is_valid_;
}

std::istream& Package::loadFromStream(std::istream& input_stream, Package& package) {
    package.is_valid_ = false;

    PackageHeader header;
    if(!PackageHeader::loadFromStream(input_stream, header)) {
        return input_stream;
    }

    package.is_valid_ = true;
    return input_stream;
}

std::ostream& Package::saveToStream(std::ostream& output_stream, const Package& package) {
    return output_stream;
}

}