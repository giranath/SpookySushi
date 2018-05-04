add_library(sushi-app OBJECT
        base_game.hpp
        game_loop.cpp
        game_loop.hpp
        )
target_include_directories(sushi-app
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PUBLIC ${GL3W_INCLUDE_DIRS}
        PUBLIC ${TOML11_INCLUDE_DIRS})