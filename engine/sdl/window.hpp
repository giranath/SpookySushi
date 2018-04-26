#ifndef SPOOKYSUSHI_WINDOW_HPP
#define SPOOKYSUSHI_WINDOW_HPP

#include <SDL.h>
#include <string>
#include <string_view>

namespace sushi {

class window_builder;

class window {
    friend window_builder;

    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_ctx_ = {};

    explicit window(SDL_Window* owned) noexcept;
public:
    window() noexcept = default;
    window(const window&) = delete;
    window& operator=(const window&) = delete;
    window(window&& other) noexcept;
    window& operator=(window&& other) noexcept;
    ~window() noexcept;

    void swap(window& other) noexcept;

    explicit operator SDL_Window*() noexcept;
};

class window_builder {
    std::string title;
    int width;
    int height;
    int x;
    int y;
    uint32_t flags;
public:
    explicit window_builder(std::string_view title);

    window_builder& with_dimensions(int width, int height) noexcept;
    window_builder& with_position(int x, int y) noexcept;
    window_builder& with_centered_position() noexcept;
    window_builder& with_width(int w) noexcept;
    window_builder& with_height(int h) noexcept;
    window_builder& with_opengl() noexcept;
    window_builder& as_fullscreen() noexcept;
    window_builder& as_desktop_fullscreen() noexcept;
    window_builder& as_resizable() noexcept;
    window_builder& as_borderless() noexcept;

    window build();
};

}

#endif //SPOOKYSUSHI_WINDOW_HPP
