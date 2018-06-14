#ifndef SPOOKYSUSHI_DEBUG_RESPONSE_HPP
#define SPOOKYSUSHI_DEBUG_RESPONSE_HPP

#include <vector>
#include <cstdint>
#include <iostream>

namespace sushi { namespace debug {

class response {
public:
    // 100 <= status < 200 => Success
    // 200 <= status < 300 => Client-side errors
    // 300 <= status < 400 => Server-side errors
    enum status {
        ok                  = 100,  // Operation succeed
        created             = 101,  // Resource created
        deleted             = 102,  // Resource deleted
        unauthorized        = 200,  // Not authorized to proceed
        not_found           = 201,  // Resource not found
        method_not_allowed  = 202,  // Resource found, but specified method is not allowed
        not_acceptable      = 203,  // Request is ill-formed
        not_implemented     = 300,  // Method not implemented on resource
        internal_error      = 301   // An internal error happened
    };

    explicit response(status stat);
    response(status stat, const std::vector<uint8_t>& payload);
    template<typename It>
    response(status stat, It begin, It end)
    : status_{stat}
    , payload_{begin, end} {

    }

    friend std::ostream& operator<<(std::ostream& os, const response& res);
    friend std::istream& operator>>(std::istream& is, response& res);

private:
    status status_;
    std::vector<uint8_t> payload_;
};

}}

#endif
