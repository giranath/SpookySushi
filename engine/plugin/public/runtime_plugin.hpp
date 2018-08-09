#ifndef SPOOKYSUSHI_RUNTIME_PLUGIN_HPP
#define SPOOKYSUSHI_RUNTIME_PLUGIN_HPP

#include "plugin_error_codes.h"
#include <cassert>

namespace sushi {

using ApiHandle = void*;

/**
 * Base class for every runtime plugins
 */
class RuntimePlugin {
public:
    enum class Status {
        Inactive,
        Active
    };
private:
    Status current_state;
public:
    RuntimePlugin() noexcept
    : current_state{Status::Inactive} {

    }

    virtual ~RuntimePlugin() noexcept {
        // Verify that the plugin has been made inactive
        assert(current_state == Status::Inactive);
    }

    /**
     * Starts the plugin
     * @return 0 on success or an error code
     */
    virtual int start() {
        current_state = Status::Active;
        return PLUGIN_OK;
    }

    /**
     * Stops the plugin.
     * The plugin should release all it's resource
     */
    virtual void stop() {
        current_state = Status::Inactive;
    }

    /**
     * Check if the plugin is currently active
     * @return true when active or false otherwise
     */
    bool is_active() const noexcept {
        return current_state == Status::Active;
    }

    /**
     * Returns the structure that defines
     * @param version The plugin version to retrieve
     * @return a structure that exposes the plugin's api
     */
    virtual ApiHandle get_api(int version) = 0;
};

}

#endif
