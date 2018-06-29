#ifndef SPOOKYSUSHI_INPUT_PROCESSOR_HPP
#define SPOOKYSUSHI_INPUT_PROCESSOR_HPP

#include <vector>

namespace sushi {

class InputEvent;
class InputContext;

class InputProcessor {
    InputContext* current_context = nullptr;
public:
    InputProcessor() = default;

    void process(const std::vector<const InputEvent*>& events);

    void use(InputContext* context) noexcept;
};

class InputProcessorFacade {
    InputProcessor& processor;
public:
    InputProcessorFacade(InputProcessor& processor);
    void use(InputContext* context) noexcept;
};

}

#endif //SPOOKYSUSHI_INPUT_PROCESSOR_HPP
