#ifndef SPOOKYSUSHI_WINDOW_HPP
#define SPOOKYSUSHI_WINDOW_HPP

#include "display.hpp"

#include <SDL.h>
#include <string>
#include <string_view>

namespace sushi {

class WindowBuilder;

class Window {
    friend WindowBuilder;

    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_ctx_ = {};

    explicit Window(SDL_Window* owned) noexcept;
public:
    Window() noexcept = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;
    ~Window() noexcept;

    void swap(Window& other) noexcept;

    void hide() noexcept;
    void show() noexcept;
    void minimize() noexcept;
    void maximize() noexcept;
    void restore() noexcept;
    void raise() noexcept;

    Display current_display() noexcept;

    explicit operator SDL_Window*() noexcept;
};

class WindowBuilder {
    std::string title;
    int width;
    int height;
    int x;
    int y;
    uint32_t flags;
public:
    explicit WindowBuilder(std::string_view title);

    WindowBuilder& with_dimensions(int width, int height) noexcept;
    WindowBuilder& with_position(int x, int y) noexcept;
    WindowBuilder& with_centered_position() noexcept;
    WindowBuilder& with_width(int w) noexcept;
    WindowBuilder& with_height(int h) noexcept;
    WindowBuilder& with_opengl() noexcept;
    WindowBuilder& as_fullscreen() noexcept;
    WindowBuilder& as_desktop_fullscreen() noexcept;
    WindowBuilder& as_resizable() noexcept;
    WindowBuilder& as_borderless() noexcept;

    Window build();
};

}

#endif //SPOOKYSUSHI_WINDOW_HPP
