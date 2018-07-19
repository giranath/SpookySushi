#ifndef SPOOKYSUSHI_NETWORK_TCP_SOCKET_HPP
#define SPOOKYSUSHI_NETWORK_TCP_SOCKET_HPP

#include "endpoint.hpp"
#include "event.hpp"
#include <memory>
#include <cstddef>
#include <chrono>

namespace sushi { namespace network {

class tcp_socket {
    class impl;
    std::unique_ptr<impl> pimpl;
    bool is_connected;
    int last_error;

    bool set_non_blocking() noexcept;
    bool set_blocking() noexcept;

    void internal_disconnect();

public:
    enum class disconnection_cause {
        remote_disconnected,
        local_disconnected
    };

    event<const tcp_socket&, disconnection_cause> on_disconnection;

    tcp_socket();
    ~tcp_socket();
    explicit tcp_socket(const ipv4_endpoint& endpoint);
    explicit tcp_socket(const ipv6_endpoint& endpoint);

    bool try_connect(const ipv4_endpoint& endpoint);
    bool try_connect(const ipv4_endpoint& endpoint, std::chrono::microseconds timeout);
    template<typename Duration>
    bool try_connect(const ipv4_endpoint& endpoint, Duration timeout) {
        return try_connect(endpoint, std::chrono::duration_cast<std::chrono::microseconds>(timeout));
    }
    bool try_connect(const ipv6_endpoint& endpoint);
    void disconnect();

    std::size_t send(const char* data, std::size_t len) noexcept;
    std::size_t wait(char* dest, std::size_t len) noexcept;

    bool good() const noexcept;
    bool connected() const noexcept;
    int error() const noexcept;

    operator bool() const noexcept;
};

}}

#endif
