#include "game.hpp"
#include <game_loop.hpp>

int main(int argc, char* argv[]) {
    Game g;

    return sushi::run_game(g, sushi::get_arguments(argc, argv));
}