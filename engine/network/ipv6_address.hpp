#ifndef SPOOKYSUSHI_NETWORK_IPV6_ADDRESS_HPP
#define SPOOKYSUSHI_NETWORK_IPV6_ADDRESS_HPP

#include <cstdint>
#include <string>

namespace sushi { namespace network {

class ipv6_address {
    uint8_t bytes[16];
public:
    ipv6_address() noexcept; // Invalid address
    ipv6_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
                 uint8_t e, uint8_t f, uint8_t g, uint8_t i,
                 uint8_t j, uint8_t k, uint8_t l, uint8_t m,
                 uint8_t n, uint8_t o, uint8_t p, uint8_t q) noexcept;
    explicit ipv6_address(uint8_t addr[16]) noexcept;
    explicit ipv6_address(const char* addr) noexcept;

    bool valid() const noexcept;
    explicit operator bool() const noexcept;

    bool operator==(const ipv6_address& other) const noexcept;
    bool operator!=(const ipv6_address& other) const noexcept;

    const uint8_t* network_value() const noexcept;

    friend std::string to_string(const ipv6_address& address);

    static const ipv6_address any;
    static const ipv6_address loopback;
};

}}

#endif
