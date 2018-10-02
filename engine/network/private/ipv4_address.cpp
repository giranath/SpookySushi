#include "ipv4_address.hpp"

#include "driver.hpp"

namespace sushi { namespace network {

ipv4_address::ipv4_address() noexcept
: address_(0) {

}

ipv4_address::ipv4_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d) noexcept
: address_((a << 24) | (b << 16) | (c << 8) | d) {

}

ipv4_address::ipv4_address(uint32_t address) noexcept
: address_(address) {

}

ipv4_address::ipv4_address(const char* address) noexcept
: address_() {
    uint32_t network_address;
    inet_pton(AF_INET, address, &network_address);

    address_ = ntohl(network_address);
}

bool ipv4_address::valid() const noexcept {
    return address_ != 0;
}

ipv4_address::operator bool() const noexcept
{
    return valid();
}

bool ipv4_address::operator==(const ipv4_address& other) const noexcept {
    return address_ == other.address_;
}

bool ipv4_address::operator!=(const ipv4_address& other) const noexcept {
    return address_ != other.address_;
}

uint32_t ipv4_address::network_value() const noexcept {
    return htonl(address_);
}

std::string to_string(const ipv4_address& address) {
    in_addr addr;
    addr.s_addr = address.address_;

    return std::string(inet_ntoa(addr));
}

#if _WIN32
const ipv4_address ipv4_address::any{};
const ipv4_address ipv4_address::loopback{};
#else
const ipv4_address ipv4_address::any{INADDR_ANY};
const ipv4_address ipv4_address::loopback{INADDR_LOOPBACK};
#endif

}}