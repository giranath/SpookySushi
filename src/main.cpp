#include "../engine/sdl/window.hpp"
#include <SDL.h>

#include <iostream>

int main() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "cannot initialize SDL: " << SDL_GetError() << std::endl;

        return 1;
    }

    sushi::window_builder builder("SpookySushi");
    sushi::window main_window = builder.with_centered_position()
            .with_dimensions(800, 600)
            .with_opengl()
            .build();

    bool is_running = true;
    while(is_running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                is_running = false;
            }
        }

    }

    SDL_Quit();
}