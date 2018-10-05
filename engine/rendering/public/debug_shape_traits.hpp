#ifndef SPOOKYSUSHI_DEBUG_SHAPE_TRAITS_HPP
#define SPOOKYSUSHI_DEBUG_SHAPE_TRAITS_HPP

#include <cstdint>

namespace sushi {

template<typename ShapeType>
struct debug_shape_traits {
    static const uint32_t vertice_count = 0;
};

#define IMPL_DEBUG_SHAPE_TRAITS(shape, count) template<> struct debug_shape_traits<shape> { static const uint32_t vertice_count = count; };


};

#endif //SPOOKYSUSHI_DEBUG_SHAPE_TRAITS_HPP
