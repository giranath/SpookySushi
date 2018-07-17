#include "input_processor.hpp"
#include "../service/input_bus_service.hpp"

#include <algorithm>
#include <iterator>

namespace sushi {

void InputProcessor::process() const noexcept {
    InputBusReader& inputs_reader = InputBusService::get();

    for(BaseInputProcessor* processor : processors) {
        processor->process(inputs_reader.begin(), inputs_reader.end());
    }
}

void InputProcessor::register_processor(BaseInputProcessor* processor) {
    processors.push_back(processor);
}

void InputProcessor::unregister_processor(BaseInputProcessor* processor) {
    auto it = std::find(std::begin(processors), std::end(processors), processor);

    if(it != std::end(processors)) {
        std::swap(*it, processors.back());
        processors.pop_back();
    }
}

bool InputProcessor::is_registered(BaseInputProcessor* processor) const noexcept {
    return std::find(std::begin(processors), std::end(processors), processor) != std::end(processors);
}

}