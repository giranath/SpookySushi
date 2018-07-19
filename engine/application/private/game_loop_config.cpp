#include "game_loop_config.hpp"

namespace sushi {

void from_xml(const XmlNode& xml_node, GameLoopConfig& config) noexcept {
    XmlNode* window_xml_node = xml_node.first_node("window");
    XmlNode* jobs_xml_node = xml_node.first_node("jobs");

    if(window_xml_node) {
       from_xml(*window_xml_node, config.window);
    }

    if(jobs_xml_node) {
        from_xml(*jobs_xml_node, config.jobs);
    }
}

void to_xml(XmlNode& xml_node, const GameLoopConfig& config) noexcept {

}

}