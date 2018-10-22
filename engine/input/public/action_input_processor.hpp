#ifndef SPOOKYSUSHI_ACTION_INPUT_PROCESSOR_HPP
#define SPOOKYSUSHI_ACTION_INPUT_PROCESSOR_HPP

#include "input_processor.hpp"
#include "input_event.hpp"

namespace sushi {

class KeyActionInputProcessor : public ActionInputProcessor {
    Key key;
    bool active;
    bool is_pressed;
public:
    KeyActionInputProcessor(Key key);

    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
    bool is_active() const override;
    void reset() override;
};

}

#endif //SPOOKYSUSHI_ACTION_INPUT_PROCESSOR_HPP
