#include "../engine/sdl/window.hpp"
#include "../engine/sdl/event.hpp"
#include "game_loop.hpp"
#include <SDL.h>

sushi::window create_window() {
    sushi::window_builder builder("SpookySushi");
    return builder.with_centered_position()
            .with_dimensions(800, 600)
            .with_opengl()
            .build();
}

int main() {
    game_loop loop;
    sushi::window main_window = create_window();
    loop.run([&main_window]() {
        for(const SDL_Event& ev : sushi::poll_event_iterator{}) {
            if(ev.type == SDL_QUIT) {
                // Loop is interrupted
                return false;
            }
        }

        // Proceed to loop again
        return true;
    });
}