#ifndef SPOOKYSUSHI_AXIS_INPUT_PROCESSOR_HPP
#define SPOOKYSUSHI_AXIS_INPUT_PROCESSOR_HPP

#include "input_processor.hpp"
#include "input_event.hpp"

namespace sushi {

class KeyAxisInputProcessor : public AxisInputProcessor {
    Key positive_key;
    Key negative_key;
    bool is_positive_pressed;
    bool is_negative_pressed;
public:
    KeyAxisInputProcessor(Key positive, Key negative) noexcept;

    void set_keys(Key positive, Key negative) noexcept;
    void set_positive_key(Key key) noexcept;
    void set_negative_key(Key key) noexcept;

    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
    float value() const override;
};

enum class MouseAxis {
    Vertical,
    Horizontal
};

class MouseAxisInputProcessor : public AxisInputProcessor {
    MouseAxis axis;
    float current_value;
    float sensibility;
public:
    MouseAxisInputProcessor(MouseAxis axis, float sensibility = 1.f) noexcept;

    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
    float value() const override;
    bool is_normalized() const override;
    void set_sensibility(float new_sensibility) noexcept;
};

}


#endif
