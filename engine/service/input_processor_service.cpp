#include "input_processor_service.hpp"

namespace sushi {

InputProcessorFacade* InputProcessorService::location = nullptr;

void InputProcessorService::locate(InputProcessorFacade* processor) {
    location = processor;
}

InputProcessorFacade& InputProcessorService::get() noexcept {
    return *location;
}

}