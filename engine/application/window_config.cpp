#include "window_config.hpp"
#include "../service/log_service.hpp"

#include <algorithm>
#include <iterator>
#include <locale>

namespace sushi {

// int WindowConfig::

WindowConfig::WindowConfig() noexcept
: title{}
, width{800}
, height{600}
, min_width{0}
, min_height{0}
, max_width{NoMaximumLimit}
, max_height{NoMaximumLimit}
, allow_resize{true}
, fullscreen{FullscreenMode::none} {

}

bool WindowConfig::has_maximum_width() const noexcept {
    return max_width != NoMaximumLimit;
}

bool WindowConfig::has_maximum_height() const noexcept {
    return max_height != NoMaximumLimit;
}

bool WindowConfig::has_minimum_width() const noexcept {
    return min_width != 0;
}

bool WindowConfig::has_minimum_height() const noexcept {
    return min_height != 0;
}

void from_xml(const XmlNode& xml_node, WindowConfig& config) noexcept {
    XmlAttribute* title_attribute = xml_node.first_attribute("title");
    if(title_attribute) {
        config.title = title_attribute->value();
    }
    else {
        log_critical("sushi.config.window", "missing title attribute");
    }

    XmlNode* size_node = xml_node.first_node("size");
    if(size_node) {
        parse_xml_attribute<int>(size_node->first_attribute("initial_width"),  config.width,       800);
        parse_xml_attribute<int>(size_node->first_attribute("initial_height"), config.height,      600);
        parse_xml_attribute<int>(size_node->first_attribute("min_width"),      config.min_width,   0);
        parse_xml_attribute<int>(size_node->first_attribute("min_height"),      config.min_height, 0);
        parse_xml_attribute<int>(size_node->first_attribute("max_width"),       config.max_width,  WindowConfig::NoMaximumLimit);
        parse_xml_attribute<int>(size_node->first_attribute("max_height"),      config.max_height, WindowConfig::NoMaximumLimit);

        XmlAttribute* fullscreen_mode_attribute = size_node->first_attribute("fullscreen_mode");
        if(fullscreen_mode_attribute) {
            //std::string fullscreen_mode
            std::string fullscreen_mode{fullscreen_mode_attribute->value()};

            std::transform(std::begin(fullscreen_mode), std::end(fullscreen_mode), std::begin(fullscreen_mode), [](char letter) {
                return std::tolower(letter);
            });

            using FullscreenMode = WindowConfig::FullscreenMode;

            if(fullscreen_mode == "none") {
                config.fullscreen = FullscreenMode::none;
            }
            else if(fullscreen_mode == "desktop") {
                config.fullscreen = FullscreenMode::desktop;
            }
            else if(fullscreen_mode == "full") {
                config.fullscreen = FullscreenMode::full;
            }
            else {
                config.fullscreen = WindowConfig::FullscreenMode::none;
                log_warning("sushi.config.window", "invalid fullscreen mode, will use \"none\" instead");
            }
        }
    }
}

void to_xml(XmlNode& xml_node, const WindowConfig& config) noexcept {
    // TODO: Create a node
}

}