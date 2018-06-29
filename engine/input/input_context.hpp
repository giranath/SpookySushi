#ifndef SPOOKYSUSHI_INPUT_CONTEXT_HPP
#define SPOOKYSUSHI_INPUT_CONTEXT_HPP

#include <memory>
#include <vector>

namespace sushi {

class InputHandler;
class InputEvent;

class InputContext {
    InputContext* parent;
    std::vector<InputHandler*> handlers;
public:
    InputContext();
    InputContext(InputContext& parent);

    void handle(const InputEvent* event);
    void register_handler(InputHandler* handler);
    void unregister_handler(InputHandler* handler);
    void reset();
};

}

#endif //SPOOKYSUSHI_INPUT_CONTEXT_HPP
