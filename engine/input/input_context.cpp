#include "input_context.hpp"
#include "input_handler.hpp"

#include <algorithm>
#include <iterator>

namespace sushi {

InputContext::InputContext()
: parent{nullptr} {

}

InputContext::InputContext(InputContext& parent)
: parent{&parent} {

}

void InputContext::handle(const InputEvent* event) {
    auto handles_event = [event](InputHandler*& handler) { return handler->handle(event); };

    if(std::none_of(std::begin(handlers), std::end(handlers), handles_event) && parent) {
        parent->handle(event);
    }
}

void InputContext::register_handler(sushi::InputHandler* handler) {
    handlers.push_back(handler);
}

void InputContext::unregister_handler(InputHandler* handler) {
    auto it = std::find(std::begin(handlers), std::end(handlers), handler);

    if(it != std::end(handlers)) {
        std::swap(*it, handlers.back());
        handlers.pop_back();
    }
}

void InputContext::reset() {
    handlers.clear();
}

}