#include "driver.hpp"

namespace sushi { namespace driver { namespace socket {

struct WSADATA g_wsadata;

bool start() noexcept {
    int result = WSAStartup(MAKEWORD(2, 2), &g_wsadata);

    return result == 0;
}

void stop() noexcept {
    WSACleanup();
}

int close_socket(socket_handle socket) noexcept {
    return ::closesocket(socket);
}

}}}
