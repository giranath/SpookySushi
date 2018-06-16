#ifndef SPOOKYSUSHI_DEBUG_REQUEST_HPP
#define SPOOKYSUSHI_DEBUG_REQUEST_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

// TODO: Serialization facilities

namespace sushi { namespace debug {

class request {
public:
    enum class method : uint8_t {
        create,
        retrieve,
        modify,
        destroy,
        unknown
    };

    request() noexcept;
    request(method action, const std::string& url);
    request(method action, const std::string& url, const std::vector<uint8_t>& payload);

    template<typename It>
    request(method action, const std::string& url, It begin, It end)
    : request(action, url, std::vector<uint8_t>(begin, end)) {
    }

    friend std::ostream& operator<<(std::ostream& os, const request& req) noexcept;
    friend std::istream& operator>>(std::istream& is, request& req) noexcept;

private:
    std::string url_;
    std::string token_;
    method method_;
    uint8_t version_;
    std::vector<uint8_t> payload;
};

std::ostream& operator<<(std::ostream& os, const request::method& method) noexcept;
std::istream& operator>>(std::istream& is, request::method& method) noexcept;

// METHOD URL VERSION\n
// TOKEN\n
// SIZE\n
// PAYLOAD

}}

#endif
