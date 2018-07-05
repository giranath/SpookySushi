#ifndef SPOOKYSUSHI_KEYSTATEINPUTHANDLER_HPP
#define SPOOKYSUSHI_KEYSTATEINPUTHANDLER_HPP

#include "input_handler.hpp"

namespace sushi {

class KeyStateInputHandler : public StateInputHandler {
    Key key;
    bool state;
public:
    explicit KeyStateInputHandler(Key key);
    bool handle(const InputEvent* event) override;
    bool is_active() const override;
};


}

#endif
