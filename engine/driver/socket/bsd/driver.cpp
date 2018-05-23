#include "driver.hpp"

#include <unistd.h>
#include <iostream>

namespace sushi { namespace driver { namespace socket {

bool start() noexcept {
    return true;
}

void stop() noexcept {
    // NOTHING
}

int get_socket_error(socket_handle handle) {
    int err = -1;

    socklen_t len = sizeof(err);
    if(getsockopt(handle, SOL_SOCKET, SO_ERROR, (char*)&err, &len) == -1) {
        throw std::system_error(std::error_code(errno, std::system_category()));
    }

    if(err) {
        errno = err;
    }

    return err;
}

int close_socket(socket_handle handle) noexcept {
    if(handle >= 0) {
        get_socket_error(handle);
        if (::shutdown(handle, SHUT_RDWR) < 0) {
            if(errno != ENOTCONN && errno != EINVAL) {
                std::cerr << "failed to shutdown socket: " << errno << std::endl;
            }
        }

        return ::close(handle);
    }

    errno = EINVAL;
    return -1;
}

}}}
