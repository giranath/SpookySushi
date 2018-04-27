#ifndef SPOOKYSUSHI_GAME_LOOP_HPP
#define SPOOKYSUSHI_GAME_LOOP_HPP

#include "../engine/async/engine.hpp"
#include "../engine/service/job_service.hpp"

#include <SDL.h>
#include <stdexcept>

class game_loop {
    sushi::async::engine jobs_;
public:
    game_loop()
    : jobs_(std::thread::hardware_concurrency() - 1, 2048){
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw std::runtime_error("cannot initialize SDL");
        }
    }

    template<typename FN>
    void run(FN fn) {
        sushi::jobs_service::locate(&jobs_);
        jobs_.start();

        bool is_running = true;
        while(is_running) {
            is_running = fn();
        }

        jobs_.stop();
    }

    ~game_loop() {
        SDL_Quit();
    }
};

#endif //SPOOKYSUSHI_GAME_LOOP_HPP
