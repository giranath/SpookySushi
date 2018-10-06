#ifndef SPOOKYSUSHI_DEBUG_SHAPE_TRAITS_HPP
#define SPOOKYSUSHI_DEBUG_SHAPE_TRAITS_HPP

#include <cstdint>

namespace sushi {

enum class debug_shape_primitive {
    Lines,
    Triangles,
    Unknown
};

template<typename ShapeType>
struct debug_shape_traits {
    static const uint32_t vertice_count = 0;
    static const debug_shape_primitive primitive = debug_shape_primitive::Unknown;
};

#define IMPL_DEBUG_SHAPE_TRAITS(shape, count, prim)      \
template<>                                               \
struct debug_shape_traits<shape>                         \
{                                                        \
    static const uint32_t vertice_count = count;         \
    static const debug_shape_primitive primitive = prim; \
};


};

#endif //SPOOKYSUSHI_DEBUG_SHAPE_TRAITS_HPP
