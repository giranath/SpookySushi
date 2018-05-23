#ifndef SPOOKYSUSHI_IPV4_ADDRESS_HPP
#define SPOOKYSUSHI_IPV4_ADDRESS_HPP

#include <cstdint>
#include <string>

namespace sushi { namespace network {

class ipv4_address {
    uint32_t address_;
public:
    ipv4_address() noexcept; // Invalid address
    ipv4_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d) noexcept;
    explicit ipv4_address(uint32_t address) noexcept;
    explicit ipv4_address(const char* address) noexcept;

    bool valid() const noexcept;
    explicit operator bool() const noexcept;

    bool operator==(const ipv4_address& other) const noexcept;
    bool operator!=(const ipv4_address& other) const noexcept;

    uint32_t network_value() const noexcept;

    friend std::string to_string(const ipv4_address& address);

    static const ipv4_address any;
    static const ipv4_address loopback;
};

}}

#endif //SPOOKYSUSHI_IPV4_ADDRESS_HPP
