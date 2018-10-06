#ifndef SPOOKYSUSHI_RIGID_BODY_HPP
#define SPOOKYSUSHI_RIGID_BODY_HPP

#include <memory>

namespace sushi {

template<typename T>
struct is_rigid_body {
    static const bool value = false;
};

template<typename T>
struct rigid_body_traits {
    using value_type = T;
};

}

#endif
