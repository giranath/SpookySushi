#ifndef SPOOKYSUSHI_NETWORK_ENDPOINT_HPP
#define SPOOKYSUSHI_NETWORK_ENDPOINT_HPP

#include "ipv4_address.hpp"
#include "ipv6_address.hpp"

#include <cstdint>

namespace sushi { namespace network {

class base_endpoint {
    uint16_t port_;

protected:
    explicit base_endpoint(uint16_t port) noexcept;
public:
    uint16_t port() const noexcept;
    uint16_t network_port() const noexcept;

    virtual bool valid() const noexcept = 0;
    explicit operator bool() const noexcept { return valid(); }
};

class ipv4_endpoint : public base_endpoint
{
    ipv4_address address_;
public:
    ipv4_endpoint() noexcept; // Invalid endpoint
    ipv4_endpoint(ipv4_address address, uint16_t port);

    bool operator==(const ipv4_endpoint& other) const noexcept;
    bool operator!=(const ipv4_endpoint& other) const noexcept;

    bool valid() const noexcept override;

    ipv4_address address() const noexcept;

    friend std::string to_string(const ipv4_endpoint& endpoint);
};

class ipv6_endpoint : public base_endpoint
{
    ipv6_address address_;
public:
    ipv6_endpoint() noexcept;  // Invalid endpoint
    ipv6_endpoint(ipv6_address address, uint16_t port);

    bool operator==(const ipv6_endpoint& other) const noexcept;
    bool operator!=(const ipv6_endpoint& other) const noexcept;

    bool valid() const noexcept override;

    ipv6_address address() const noexcept;

    friend std::string to_string(const ipv6_endpoint& endpoint);
};

}}


#endif
