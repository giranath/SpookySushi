#include "window.hpp"
#include <algorithm>

namespace sushi {

Window::Window(SDL_Window* owned) noexcept
: window_(owned) {

}

Window::Window(Window&& other) noexcept
: window_(other.window_){
    other.window_ = nullptr;
}

Window& Window::operator=(Window&& other) noexcept {
    swap(other);

    return *this;
}

Window::~Window() noexcept {
    if(gl_ctx_) {
        SDL_GL_DeleteContext(gl_ctx_);
    }

    if(window_) {
        SDL_DestroyWindow(window_);
    }
}

void Window::swap(Window& other) noexcept {
    using std::swap;
    swap(window_, other.window_);
}

void Window::hide() noexcept {
    SDL_HideWindow(window_);
}

void Window::show() noexcept {
    SDL_ShowWindow(window_);
}

void Window::minimize() noexcept {
    SDL_MinimizeWindow(window_);
}

void Window::maximize() noexcept {
    SDL_MaximizeWindow(window_);
}

void Window::restore() noexcept {
    SDL_RestoreWindow(window_);
}

void Window::raise() noexcept {
    SDL_RaiseWindow(window_);
}

Display Window::current_display() noexcept {
    return Display{SDL_GetWindowDisplayIndex(window_)};
}

Window::operator SDL_Window*() noexcept {
    return window_;
}

WindowBuilder::WindowBuilder(std::string_view title)
: title{title}
, width(800)
, height(600)
, x(SDL_WINDOWPOS_UNDEFINED)
, y(SDL_WINDOWPOS_UNDEFINED)
, flags(0) {

}

WindowBuilder& WindowBuilder::with_dimensions(int width, int height) noexcept {
    this->width = width;
    this->height = height;

    return *this;
}

WindowBuilder& WindowBuilder::with_position(int x, int y) noexcept {
    this->x = x;
    this->y = y;

    return *this;
}

WindowBuilder& WindowBuilder::with_centered_position() noexcept {
    x = SDL_WINDOWPOS_CENTERED;
    y = SDL_WINDOWPOS_CENTERED;

    return *this;
}

WindowBuilder& WindowBuilder::with_width(int w) noexcept {
    width = w;

    return *this;
}

WindowBuilder& WindowBuilder::with_height(int h) noexcept {
    height = h;

    return *this;
}

WindowBuilder& WindowBuilder::with_opengl() noexcept {
    flags |= SDL_WINDOW_OPENGL;

    return *this;
}

WindowBuilder& WindowBuilder::as_fullscreen() noexcept {
    flags |= SDL_WINDOW_FULLSCREEN;

    return *this;
}

WindowBuilder& WindowBuilder::as_desktop_fullscreen() noexcept {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    return *this;
}

WindowBuilder& WindowBuilder::as_resizable() noexcept {
    flags |= SDL_WINDOW_RESIZABLE;

    return *this;
}

WindowBuilder& WindowBuilder::as_borderless() noexcept {
    flags |= SDL_WINDOW_BORDERLESS;

    return *this;
}

Window WindowBuilder::build() {
    Window w{SDL_CreateWindow(title.c_str(), x, y, width, height, flags)};
    if(flags & SDL_WINDOW_OPENGL) {
        w.gl_ctx_ = SDL_GL_CreateContext(w.window_);
    }

    return w;
}

}