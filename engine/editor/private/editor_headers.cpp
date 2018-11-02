#include "editor_headers.hpp"
#include <endian.hpp>
#include <limits>

namespace sushi { namespace editor {

static const char HEADER_MAGIC[4] = {'S', 'E', 'R', 'M'};

std::ostream& operator<<(std::ostream& out, const RequestHeader& header) noexcept {
    if(header.endpoint_url.size() > std::numeric_limits<uint16_t>::max()) {
        out.setstate(std::ios::failbit);
        return out;
    }

    const uint16_t url_length = static_cast<const uint16_t>(header.endpoint_url.size());

    const char RESERVED[6] = { 'S', 'U', 'S', 'H', 'I', '!'};

    const uint16_t url_length_big_endian = to_network_endian(url_length);
    const uint16_t request_id_big_endian = to_network_endian(header.request_id);
    const uint64_t body_length_to_big_endian = to_network_endian(header.body_length);

    out.write(HEADER_MAGIC, 4);
    out.write(reinterpret_cast<const char*>(&header.version), 1);
    out.write(reinterpret_cast<const char*>(&header.body_type), 1);
    out.write(reinterpret_cast<const char*>(&url_length_big_endian), 2);
    out.write(reinterpret_cast<const char*>(&request_id_big_endian), 2);
    out.write(reinterpret_cast<const char*>(RESERVED), sizeof(RESERVED));
    out.write(reinterpret_cast<const char*>(&url_length_big_endian), 8);
    out.write(header.endpoint_url.data(), url_length);

    return out;
}

std::istream& operator<<(std::istream& in, RequestHeader& header) noexcept {
    char magic[4] = {0};
    in.read(magic, 4);

    for(uint8_t i = 0u; i < 4u; ++i) {
        if(magic[i] != HEADER_MAGIC[i]) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    uint16_t url_length = 0;
    uint16_t request_id = 0;
    uint64_t body_length = 0;

    // The magic is valid
    in.read(reinterpret_cast<char*>(&header.version), 1);
    in.read(reinterpret_cast<char*>(&header.body_type), 1);
    in.read(reinterpret_cast<char*>(&url_length), 2);
    url_length = from_network_endian(url_length);

    in.read(reinterpret_cast<char*>(&request_id), 2);
    header.request_id = from_network_endian(request_id);

    in.ignore(5);
    in.read(reinterpret_cast<char*>(&body_length), 8);
    header.body_length = from_network_endian(body_length);

    in.read(header.endpoint_url.data(), url_length);

    return in;
}

std::ostream& operator<<(std::ostream& out, const ResponseHeader& header) noexcept {
    if(header.endpoint_url.size() > std::numeric_limits<uint16_t>::max()) {
        out.setstate(std::ios::failbit);
        return out;
    }

    const uint16_t url_length = static_cast<const uint16_t>(header.endpoint_url.size());

    const char RESERVED[5] = { 'S', 'U', 'S', 'H', 'I' };

    out.write(HEADER_MAGIC, 4);
    out.write(reinterpret_cast<const char*>(&header.version), 1);
    out.write(reinterpret_cast<const char*>(&header.body_type), 1);
    out.write(reinterpret_cast<const char*>(&url_length), 2);
    out.write(reinterpret_cast<const char*>(&header.request_id), 2);
    out.write(reinterpret_cast<const char*>(&header.status_code), 1);
    out.write(reinterpret_cast<const char*>(RESERVED), sizeof(RESERVED));
    out.write(reinterpret_cast<const char*>(&header.body_length), 8);
    out.write(header.endpoint_url.data(), url_length);

    return out;
}

std::istream& operator<<(std::istream& in, ResponseHeader& header) noexcept {
    return in;
}

}}