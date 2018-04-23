#include "window.hpp"
#include <algorithm>

namespace sushi {

window::window(SDL_Window* owned) noexcept
: window_(owned) {

}

window::window(window&& other) noexcept
: window_(other.window_){
    other.window_ = nullptr;
}

window& window::operator=(window&& other) noexcept {
    swap(other);

    return *this;
}

window::~window() {
    if(window_) {
        SDL_DestroyWindow(window_);
    }
}

void window::swap(window& other) noexcept {
    std::swap(window_, other.window_);
}

window_builder::window_builder(std::string_view title)
: title{title}
, width(800)
, height(600)
, x(SDL_WINDOWPOS_UNDEFINED)
, y(SDL_WINDOWPOS_UNDEFINED)
, flags(0) {

}

window_builder& window_builder::with_dimensions(int width, int height) noexcept {
    this->width = width;
    this->height = height;

    return *this;
}

window_builder& window_builder::with_position(int x, int y) noexcept {
    this->x = x;
    this->y = y;

    return *this;
}

window_builder& window_builder::with_centered_position() noexcept {
    x = SDL_WINDOWPOS_CENTERED;
    y = SDL_WINDOWPOS_CENTERED;

    return *this;
}

window_builder& window_builder::with_width(int w) noexcept {
    width = w;

    return *this;
}

window_builder& window_builder::with_height(int h) noexcept {
    height = h;

    return *this;
}

window_builder& window_builder::with_opengl() noexcept {
    flags |= SDL_WINDOW_OPENGL;

    return *this;
}

window_builder& window_builder::as_fullscreen() noexcept {
    flags |= SDL_WINDOW_FULLSCREEN;

    return *this;
}

window_builder& window_builder::as_desktop_fullscreen() noexcept {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    return *this;
}

window_builder& window_builder::as_resizable() noexcept {
    flags |= SDL_WINDOW_RESIZABLE;

    return *this;
}

window_builder& window_builder::as_borderless() noexcept {
    flags |= SDL_WINDOW_BORDERLESS;

    return *this;
}

window window_builder::build() {
    return window{SDL_CreateWindow(title.c_str(), x, y, width, height, flags)};
}

}