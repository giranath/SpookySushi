#include "input_processor.hpp"
#include "input_context.hpp"

namespace sushi {

void InputProcessor::process(const std::vector<const InputEvent*>& events) {
    if(current_context == nullptr) return;

    // Processes every events
    for(const InputEvent* event : events) {
        current_context->handle(event);
    }
}

void InputProcessor::use(InputContext* context) noexcept {
    current_context = context;
}

InputProcessorFacade::InputProcessorFacade(InputProcessor& processor)
: processor{processor} {

}

void InputProcessorFacade::use(InputContext* context) noexcept {
    processor.use(context);
}

}