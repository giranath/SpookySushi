#include "request.hpp"

#include <algorithm>
#include <iterator>

namespace sushi { namespace debug {

request::request() noexcept
: method_(method::unknown)
, url_{}
, version_{1}
, payload{} {

}

request::request(method action, const std::string& url)
: method_{action}
, url_{url}
, version_{1}
, payload{} {

}

request::request(method action, const std::string& url, const std::vector<uint8_t>& payload)
: method_{action}
, url_{url}
, version_{1}
, payload{payload} {

}

std::ostream& operator<<(std::ostream& os, const request& req) noexcept {
    os << req.method_ << " " << req.url_ << " " << int(req.version_) << "\n"
       << req.token_ << "\n"
       << req.payload.size() << "\n";
    std::copy(std::begin(req.payload), std::end(req.payload), std::ostreambuf_iterator<char>(os));

    return os;
}

std::istream& operator>>(std::istream& is, request& req) noexcept {
    int version;
    std::size_t payload_size;
    is >> req.method_ >> req.url_ >> version >> req.token_ >> payload_size;

    if(version != 1) {
        // TODO: Handle invalid version
    }

    if(payload_size > 0) {
        req.payload.clear();
        std::copy_n(std::istreambuf_iterator<char>(is), payload_size, std::back_inserter(req.payload));
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const request::method& method) noexcept {
    switch(method) {
        case request::method::create:
            os << "create";
            break;
        case request::method::modify:
            os << "modify";
            break;
        case request::method::destroy:
            os << "destroy";
            break;
        case request::method::retrieve:
            os << "retrieve";
            break;
        default:
            os << "unknown";
            break;
    }

    return os;
}

std::istream& operator>>(std::istream& is, request::method& method) noexcept {
    std::string method_str;
    is >> method;
    std::transform(std::begin(method_str), std::end(method_str), std::begin(method_str),
                   [](char l) { return std::tolower(l); });

    if(method_str == "create") {
        method = request::method::create;
    }
    else if(method_str == "modify") {
        method = request::method::modify;
    }
    else if(method_str == "destroy") {
        method = request::method::destroy;
    }
    else if(method_str == "retrieve") {
        method = request::method::retrieve;
    }
    else {
        method = request::method::unknown;
    }

    return is;
}

}}