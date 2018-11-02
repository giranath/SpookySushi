#ifndef SPOOKYSUSHI_EDITOR_CONFIG_HPP
#define SPOOKYSUSHI_EDITOR_CONFIG_HPP

#include <xml.hpp>

namespace sushi {

struct EditorConfig {
};

void from_xml(const XmlNode& xml_node, EditorConfig& config) noexcept;
void to_xml(XmlNode& xml_node, const EditorConfig& config) noexcept;

}

#endif //SPOOKYSUSHI_EDITOR_CONFIG_HPP
