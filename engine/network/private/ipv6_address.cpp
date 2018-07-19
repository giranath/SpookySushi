#include "ipv6_address.hpp"

#include "driver.hpp"

#include <algorithm>
#include <iterator>

namespace sushi { namespace network {

ipv6_address::ipv6_address() noexcept {
    std::fill(std::begin(bytes), std::end(bytes), 0);
}

ipv6_address::ipv6_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
                           uint8_t e, uint8_t f, uint8_t g, uint8_t h,
                           uint8_t i, uint8_t j, uint8_t k, uint8_t l,
                           uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept {
    bytes[0]  = a;
    bytes[1]  = b;
    bytes[2]  = c;
    bytes[3]  = d;

    bytes[4]  = e;
    bytes[5]  = f;
    bytes[6]  = g;
    bytes[7]  = h;

    bytes[8]  = i;
    bytes[9]  = j;
    bytes[10] = k;
    bytes[11] = l;

    bytes[12] = m;
    bytes[13] = n;
    bytes[14] = o;
    bytes[15] = p;
}

ipv6_address::ipv6_address(uint8_t addr[16]) noexcept {
    std::copy(addr, addr + 16, std::begin(bytes));
}

ipv6_address::ipv6_address(const char* addr) noexcept {
    inet_pton(AF_INET6, addr, &bytes);
}

bool ipv6_address::valid() const noexcept {
    return false;
}

ipv6_address::operator bool() const noexcept {
    return valid();
}

bool ipv6_address::operator==(const ipv6_address& other) const noexcept {
    for(int i = 0; i < 16; ++i) {
        if(bytes[i] != other.bytes[i]) return false;
    }
    return true;
}

bool ipv6_address::operator!=(const ipv6_address& other) const noexcept {
    for(int i = 0; i < 16; ++i) {
        if(bytes[i] != other.bytes[i]) return true;
    }
    return false;
}

const uint8_t* ipv6_address::network_value() const noexcept {
    return bytes;
}

std::string to_string(const ipv6_address& address) {
    char address_str[INET6_ADDRSTRLEN];

    in6_addr addr;
    std::copy(std::begin(address.bytes), std::end(address.bytes), reinterpret_cast<char*>(&addr));
    inet_ntop(AF_INET6, &addr, address_str, sizeof(address_str));

    return std::string{address_str};
}

const ipv6_address ipv6_address::any;
const ipv6_address ipv6_address::loopback;

}}