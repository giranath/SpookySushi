#ifndef SPOOKYSUSHI_DRIVER_HPP
#define SPOOKYSUSHI_DRIVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace sushi { namespace driver { namespace socket {

using socket_handle = int;
using raw_send_buffer_type = const void*;
using raw_recv_buffer_type = void*;

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

/**
 * Closes a socket handle
 * @param handle The socket to close
 * @return error code
 */
int close_socket(socket_handle handle) noexcept;

}}}

#endif //SPOOKYSUSHI_DRIVER_HPP
