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
    explicit KeyActionInputProcessor(Key key);

    bool process(InputBusReader::const_iterator begin, InputBusReader::const_iterator end) override;
    void reset() override;
    State state() const override;
};

}

#endif //SPOOKYSUSHI_ACTION_INPUT_PROCESSOR_HPP
