#ifndef SPOOKYSUSHI_GAME_LOOP_HPP
#define SPOOKYSUSHI_GAME_LOOP_HPP

#include <SDL.h>
#include <stdexcept>

class game_loop {
public:
    game_loop() {
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw std::runtime_error("cannot initialize SDL");
        }
    }

    template<typename FN>
    void run(FN fn) {
        bool is_running = true;
        while(is_running) {
            is_running = fn();
        }
    }

    ~game_loop() {
        SDL_Quit();
    }
};

#endif //SPOOKYSUSHI_GAME_LOOP_HPP
