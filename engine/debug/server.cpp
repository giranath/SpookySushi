#include "server.hpp"

namespace sushi { namespace debug {

server::handle server::register_handler(request::method method, std::string url, request_handler handler) {
    handle this_handle = next_handle;

    return this_handle;
}

server::handle server::register_auth_handler(request::method method, std::string url, request_handler handler) {
    handle this_handle = next_handle;

    return this_handle;
}

void server::unregister(handle handler_to_register) {

}

bool server::is_registered(handle handler_to_find) const noexcept {
    return false;
}

}}