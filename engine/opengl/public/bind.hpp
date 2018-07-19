#ifndef MMAP_DEMO_BIND_HPP
#define MMAP_DEMO_BIND_HPP

namespace sushi { namespace gl {

// Traits to verify that a resource is bindable
template<typename T>
struct is_bindable {
    static const bool value = false;
};

// Bind an OpenGL resource
template<typename T>
void bind(const T& t) noexcept {
    static_assert(is_bindable<T>::value, "type must be bindable to call bind");
    t.bind();
}

}}

#endif
