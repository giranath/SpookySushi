#include "endpoint.hpp"
#include "driver.hpp"

namespace sushi { namespace network {

//======================================================================================================================
// BASE
//======================================================================================================================

base_endpoint::base_endpoint(uint16_t port) noexcept
: port_(port) {

}

uint16_t base_endpoint::port() const noexcept {
    return port_;
}

uint16_t base_endpoint::network_port() const noexcept {
    return htons(port_);
}

//======================================================================================================================
// IPV4
//======================================================================================================================

ipv4_endpoint::ipv4_endpoint() noexcept
: base_endpoint(0)
, address_() {

}

ipv4_endpoint::ipv4_endpoint(ipv4_address address, uint16_t port)
: base_endpoint(port)
, address_(address) {

}

bool ipv4_endpoint::operator==(const ipv4_endpoint& other) const noexcept {
    return port() == other.port() && address_ == other.address_;
}

bool ipv4_endpoint::operator!=(const ipv4_endpoint& other) const noexcept {
    return port() != other.port() || address_ != other.address_;
}

bool ipv4_endpoint::valid() const noexcept {
    return address_.valid();
}

ipv4_address ipv4_endpoint::address() const noexcept {
    return address_;
}

std::string to_string(const ipv4_endpoint& endpoint) {
    using std::to_string;
    return to_string(endpoint.address_) + ":" + to_string(endpoint.port());
}

//======================================================================================================================
// IPV6
//======================================================================================================================

ipv6_endpoint::ipv6_endpoint() noexcept
: base_endpoint(0)
, address_{} {

}

ipv6_endpoint::ipv6_endpoint(ipv6_address address, uint16_t port)
: base_endpoint(port)
, address_(address) {

}

bool ipv6_endpoint::operator==(const ipv6_endpoint& other) const noexcept {
    return address_ == other.address_ && port() == other.port();
}

bool ipv6_endpoint::operator!=(const ipv6_endpoint& other) const noexcept {
    return address_ != other.address_ || port() != other.port();
}

bool ipv6_endpoint::valid() const noexcept {
    return address_.valid();
}

ipv6_address ipv6_endpoint::address() const noexcept {
    return address_;
}

std::string to_string(const ipv6_endpoint& endpoint) {
    using std::to_string;

    return "[" + to_string(endpoint.address_) + "]:" + to_string(endpoint.port());
}

}}