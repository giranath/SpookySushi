#ifndef SPOOKYSUSHI_INPUT_PROCESSOR_HPP
#define SPOOKYSUSHI_INPUT_PROCESSOR_HPP

#include "input_bus.hpp"
#include <vector>

namespace sushi {

class BaseInputProcessor {
public:
    virtual ~BaseInputProcessor() = default;
    virtual void reset() {}
    virtual bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) = 0;
};

class StateInputProcessor : public BaseInputProcessor {
public:
    virtual ~StateInputProcessor() = default;
    virtual bool is_active() const = 0;

};

class AxisInputProcessor : public BaseInputProcessor {
public:
    virtual ~AxisInputProcessor() = default;
    virtual float value() const = 0;
    virtual bool is_normalized() const { return true; }
};

class ActionInputProcessor : public BaseInputProcessor {
public:
    virtual ~ActionInputProcessor() = default;
    virtual bool is_active() const = 0;
};

class InputProcessor {
    std::vector<BaseInputProcessor*> processors;
public:
    void process() const noexcept;
    void register_processor(BaseInputProcessor* processor);
    void unregister_processor(BaseInputProcessor* processor);
    bool is_registered(BaseInputProcessor* processor) const noexcept;
};

}

#endif
