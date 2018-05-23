#ifndef SPOOKYSUSHI_DRIVER_HPP
#define SPOOKYSUSHI_DRIVER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

namespace sushi { namespace driver { namespace socket {

using socket_handle = SOCKET;
using raw_send_buffer_type = const char*;
using raw_recv_buffer_type = char*;

/**
 * Starts the driver
 * @return true on success or false otherwise
 */
bool start() noexcept;

/**
 * Stop the driver
 * @return true on success or false otherwise
 */
void stop() noexcept;

int close_socket(socket_handle socket) noexcept;

}}}

#endif //SPOOKYSUSHI_DRIVER_HPP
