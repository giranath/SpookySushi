#ifndef SPOOKYSUSHI_EDITOR_HEADERS_HPP
#define SPOOKYSUSHI_EDITOR_HEADERS_HPP

#include <cstdint>
#include <string>
#include <iostream>

namespace sushi { namespace editor {

enum class BodyTypes : uint8_t {

};

enum class StatusCodes : uint8_t {

};

struct RequestHeader {
    uint64_t body_length;
    std::string endpoint_url;
    uint16_t request_id;
    uint8_t version;
    BodyTypes body_type;
};

std::ostream& operator<<(std::ostream& out, const RequestHeader& header) noexcept;
std::istream& operator<<(std::istream& in, RequestHeader& header) noexcept;

struct ResponseHeader {
    uint64_t body_length;
    std::string endpoint_url;
    uint16_t request_id;
    uint8_t version;
    StatusCodes status_code;
    BodyTypes body_type;
};

std::ostream& operator<<(std::ostream& out, const ResponseHeader& header) noexcept;
std::istream& operator<<(std::istream& in, ResponseHeader& header) noexcept;

}}

#endif //SPOOKYSUSHI_EDITOR_HEADERS_HPP
