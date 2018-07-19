#include "tcp_socket.hpp"

#include "driver.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <fcntl.h>

namespace sushi { namespace network {

struct tcp_socket::impl {
    driver::socket::socket_handle socket;

    impl()
    : socket(-1) {

    }

    ~impl() {
        if(socket != -1) {
            close();
        }
    }

    int close() noexcept {
        return driver::socket::close_socket(socket);
    }

    bool good() const noexcept {
        return socket != -1;
    }
};

bool tcp_socket::set_non_blocking() noexcept {
    int flags = fcntl(pimpl->socket, F_GETFL, 0);
    if(flags == -1) return false;

    flags = flags | O_NONBLOCK;

    return fcntl(pimpl->socket, F_SETFL, flags) == 0;
}

bool tcp_socket::set_blocking() noexcept {
    int flags = fcntl(pimpl->socket, F_GETFL, 0);
    if(flags == -1) return false;

    flags = flags & ~O_NONBLOCK;

    return fcntl(pimpl->socket, F_SETFL, flags) == 0;
}

tcp_socket::tcp_socket()
: pimpl(std::make_unique<impl>())
, is_connected(false)
, last_error(0){

}

tcp_socket::~tcp_socket() = default;

tcp_socket::tcp_socket(const ipv4_endpoint& endpoint)
: tcp_socket() {
    try_connect(endpoint);
}

tcp_socket::tcp_socket(const ipv6_endpoint& endpoint)
: tcp_socket() {
    try_connect(endpoint);
}

bool tcp_socket::try_connect(const ipv4_endpoint& endpoint, std::chrono::microseconds timeout) {
    if(connected()) {
        internal_disconnect();
    }

    // Try to create TCP socket
    pimpl->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(pimpl->socket >= 0) {
        sockaddr_in address;
        std::fill_n(reinterpret_cast<char*>(&address), sizeof(address), 0);
        address.sin_family = AF_INET;
        address.sin_port = endpoint.network_port();
        address.sin_addr.s_addr = endpoint.address().network_value();

        socklen_t len = sizeof(address);
        if(set_non_blocking()) {
            int res = ::connect(pimpl->socket, reinterpret_cast<sockaddr*>(&address), len);
            if(res == -1) {
                if(errno == EINPROGRESS) {
                    fd_set set;
                    FD_ZERO(&set);
                    FD_SET(pimpl->socket, &set);

                    int retval = 0;
                    if(timeout.count() > 0) {
                        // Setup timeout
                        const std::chrono::seconds timeout_sec = std::chrono::duration_cast<std::chrono::seconds>(
                                timeout);
                        timeval tv;
                        tv.tv_sec = timeout_sec.count();
                        tv.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(
                                timeout - timeout_sec).count();

                        retval = select(pimpl->socket + 1, nullptr, &set, nullptr, &tv);
                    }
                    else {
                        retval = select(pimpl->socket + 1, nullptr, &set, nullptr, nullptr);
                    }

                    if(retval == -1) {
                        last_error = errno;
                        internal_disconnect();
                        return false;
                    }
                    else if(retval == 0) {
                        last_error = ETIMEDOUT;
                        internal_disconnect();
                        return false;
                    }
                }
                else {
                    last_error = errno;
                    internal_disconnect();

                    return false;
                }
            }

            if(set_blocking()) {
                is_connected = true;
            }
            else {
                internal_disconnect();
                throw std::system_error(errno, std::system_category());
            }
        }
        else {
            internal_disconnect();
            throw std::system_error(errno, std::system_category());
        }
    }
    else {
        last_error = errno;
    }

    return good() && is_connected;
}

bool tcp_socket::try_connect(const ipv4_endpoint& endpoint) {
    return try_connect(endpoint, std::chrono::milliseconds(0));
}

bool tcp_socket::try_connect(const ipv6_endpoint& endpoint) {
    throw std::runtime_error("unimplemented method");
    if(pimpl->good()) {
        internal_disconnect();
    }

    // Try to create TCP socket
    pimpl->socket = ::socket(AF_INET6, SOCK_STREAM, 0);
    if(pimpl->socket >= 0) {
        sockaddr_in6 address;
        std::fill_n(reinterpret_cast<char*>(&address), sizeof(address), 0);
        address.sin6_family = AF_INET6;
        address.sin6_port = endpoint.network_port();
        // TODO: Setup address

        socklen_t len = sizeof(address);
        int res = ::connect(pimpl->socket, reinterpret_cast<sockaddr*>(&address), len);
        if(res == -1) {
            last_error = errno;
            internal_disconnect();
        }
    }
    else {
        last_error = errno;
    }

    return good();
}

void tcp_socket::disconnect() {
    internal_disconnect();
    on_disconnection(*this, disconnection_cause::local_disconnected);
}

std::size_t tcp_socket::send(const char* data, std::size_t len) noexcept {
    assert(len > 0);

    if(!connected()) return 0;

    ssize_t res = ::send(pimpl->socket, reinterpret_cast<const void*>(data), len, 0);

    if(res == -1) {
        last_error = errno;
        res = 0;
    }
    else if(res == 0) {
        is_connected = false;
        on_disconnection(*this, disconnection_cause::remote_disconnected);
    }

    return static_cast<std::size_t>(res);
}

std::size_t tcp_socket::wait(char* dest, std::size_t len) noexcept {
    assert(len > 0);

    if(!connected()) return 0;

    ssize_t res = ::recv(pimpl->socket, reinterpret_cast<void*>(dest), len, 0);

    if(res == -1) {
        last_error = errno;
    }
    else if(res == 0) {
        is_connected = false;
        on_disconnection(*this, disconnection_cause::remote_disconnected);
    }

    return static_cast<std::size_t>(res);
}

void tcp_socket::internal_disconnect() {
    if(pimpl->close() != 0) {
        last_error = errno;
    }

    pimpl->socket = -1;
    is_connected = false;
}

bool tcp_socket::good() const noexcept {
    return last_error == 0 && pimpl->socket != -1;
}

bool tcp_socket::connected() const noexcept {
    return is_connected;
}

int tcp_socket::error() const noexcept {
    return last_error;
}

tcp_socket::operator bool() const noexcept {
    return good() && connected();
}

}}