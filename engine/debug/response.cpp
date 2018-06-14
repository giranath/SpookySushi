#include "response.hpp"

#include <algorithm>
#include <iterator>

namespace sushi { namespace debug {

response::response(status stat)
: status_(stat)
, payload_{} {

}

response::response(status stat, const std::vector<uint8_t>& payload)
: status_{stat}
, payload_{payload} {

}

std::ostream& operator<<(std::ostream& os, const response& res) {
    os << res.status_ << "\n"
       << res.payload_.size() << "\n";

    std::copy(std::begin(res.payload_), std::end(res.payload_), std::ostreambuf_iterator<char>(os));

    return os;
}

std::istream& operator>>(std::istream& is, response& res) {
    std::size_t payload_size;
    int status;

    is >> status >> payload_size;
    // TODO: Check that status is valid
    res.status_ = static_cast<response::status>(status);

    if(payload_size > 0) {
        res.payload_.clear();
        std::copy_n(std::istreambuf_iterator<char>(is), payload_size, std::back_inserter(res.payload_));
    }

    return is;
}

}}