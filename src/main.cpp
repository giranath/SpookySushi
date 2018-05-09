#include "../engine/application/game_loop.hpp"
#include "../engine/ecs/entity_tree.hpp"
#include "game.hpp"
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
    sushi::ecs::entity_tree entities;
    auto it = entities.make();
    entities.make();
    entities.make();
    entities.make(entities.make(it));
    entities.remove(it);
    for(sushi::ecs::entity e : entities) {
        std::cout << e << std::endl;
    }

    game g;

    return sushi::run_game(g, get_arguments(argc, argv));
}