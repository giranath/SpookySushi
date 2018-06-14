#ifndef SPOOKYSUSHI_DEBUG_SERVER_HPP
#define SPOOKYSUSHI_DEBUG_SERVER_HPP

#include "request.hpp"
#include "response.hpp"

#include <functional>
#include <unordered_map>

namespace sushi { namespace debug {

//
// The debug server is a dev-only component of the engine
// It's role is to handle debug-requests from the outside and dispatch them to the various systems
// To achieve this, it must expose a simple way to handle requests
class server {
    using request_handler = std::function<response(const request&)>;
    using handle = std::size_t;

    struct handlers_state {
        handle identifier;
        std::string url;
        request::method method;
        bool requires_auth;
        request_handler handler;
    };

    std::vector<handlers_state> handlers;
    std::size_t next_handle;
public:
    handle register_handler(request::method method, std::string url, request_handler handler);
    handle register_auth_handler(request::method method, std::string url, request_handler handler);

    void unregister(handle handler_to_register);
    bool is_registered(handle handler_to_find) const noexcept;
};

}}

#endif
