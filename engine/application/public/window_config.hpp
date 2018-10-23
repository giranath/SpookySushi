#ifndef SPOOKYSUSHI_WINDOW_CONFIG_HPP
#define SPOOKYSUSHI_WINDOW_CONFIG_HPP

#include <xml.hpp>
#include <string>
#include <rapidxml.hpp>

namespace sushi {

struct WindowConfig {
    static constexpr int NoMaximumLimit = std::numeric_limits<int>::max();

    enum class FullscreenMode {
        none,
        desktop,
        full
    };

    std::string title;
    int width;
    int height;
    int min_width;
    int min_height;
    int max_width;
    int max_height;
    bool allow_resize;
    FullscreenMode fullscreen;

    WindowConfig() noexcept;

    bool has_maximum_width() const noexcept;
    bool has_maximum_height() const noexcept;
    bool has_minimum_width() const noexcept;
    bool has_minimum_height() const noexcept;
};

void from_xml(const XmlNode& xml_node, WindowConfig& config) noexcept;
void to_xml(XmlNode& xml_node, const WindowConfig& config) noexcept;

}

#endif //SPOOKYSUSHI_WINDOW_CONFIG_HPP
