#ifndef MMAP_DEMO_DEBUG_HPP
#define MMAP_DEMO_DEBUG_HPP

// Theses includes must be first
#include <GL/gl3w.h>
#include <SDL_opengl.h>
#include <iostream>
#include <iterator>

namespace sushi { namespace gl {

bool enable_debug_messages() noexcept;

class extension_iterator : public std::iterator<std::input_iterator_tag,
                                                std::string,
                                                std::string,
                                                const std::string*,
                                                const std::string&> {
    std::string extension;
    GLint count;
    GLint index;

    extension_iterator(const std::string &extension, GLint index, GLint count);

public:
    extension_iterator();

    extension_iterator &operator++();

    extension_iterator operator++(int);

    bool operator==(const extension_iterator &other) const noexcept;

    bool operator!=(const extension_iterator &other) const noexcept;

    pointer operator->() const noexcept;

    reference operator*() const noexcept;

    friend extension_iterator begin(extension_iterator it) noexcept;

    friend extension_iterator end(extension_iterator &it) noexcept;
};

}}

#endif
