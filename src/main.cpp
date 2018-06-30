#include "game.hpp"
#include <application/game_loop.hpp>
#include <algorithm>
#include <iterator>

std::vector<std::string> get_arguments(int argc, char* argv[]) {
    std::vector<std::string> args;
    args.reserve(static_cast<std::size_t>(argc));

    std::transform(argv, argv + argc,
                   std::back_inserter(args),
                   [](const char* a) { return std::string{a}; });

    return args;
}

int main(int argc, char* argv[]) {
    game g;

    return sushi::run_game(g, get_arguments(argc, argv));
}