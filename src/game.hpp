#ifndef SPOOKYSUSHI_GAME_HPP
#define SPOOKYSUSHI_GAME_HPP

#include <application/base_game.hpp>

class game : public sushi::base_game {
public:
    void on_start() override;
    void on_frame(sushi::frame_duration last_frame) override;
    void on_late_frame(sushi::frame_duration last_frame) override;
    void on_render(sushi::ProxyRenderer renderer) override;
    void on_stop() override;
};


#endif //SPOOKYSUSHI_GAME_HPP
