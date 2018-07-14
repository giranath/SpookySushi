#include "input_bus.hpp"
#include "input_event.hpp"

#include <iterator>

namespace sushi {

void InputBus::push(const sushi::InputEvent* event) {
    inputs.push_back(event);
}

void InputBus::clear() {
    inputs.clear();
}

InputBus::iterator InputBus::begin() {
    return std::begin(inputs);
}

InputBus::iterator InputBus::end() {
    return std::end(inputs);
}

InputBus::const_iterator InputBus::begin() const {
    return std::begin(inputs);
}

InputBus::const_iterator InputBus::end() const {
    return std::cend(inputs);
}

InputBus::const_iterator InputBus::cbegin() const {
    return begin();
}

InputBus::const_iterator InputBus::cend() const {
    return end();
}

InputBusReader::InputBusReader(InputBus& bus) noexcept
: bus{bus} {

}

InputBusReader::iterator InputBusReader::begin() const {
    return std::begin(bus);
}

InputBusReader::iterator InputBusReader::end() const {
    return std::end(bus);
}

}