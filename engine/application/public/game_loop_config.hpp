#ifndef SPOOKYSUSHI_GAME_LOOP_CONFIG_HPP
#define SPOOKYSUSHI_GAME_LOOP_CONFIG_HPP

#include "window_config.hpp"
#include "job_config.hpp"

namespace sushi {

struct GameLoopConfig {
    WindowConfig window;
    JobConfig jobs;
};

void from_xml(const XmlNode& xml_node, GameLoopConfig& config) noexcept;
void to_xml(XmlNode& xml_node, const GameLoopConfig& config) noexcept;

}

#endif //SPOOKYSUSHI_GAME_LOOP_CONFIG_HPP
