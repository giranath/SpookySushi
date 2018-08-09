#include "shared_library_runtime_plugin.hpp"
#include <log_service.hpp>

#define LOG_CATEGORY "com.sushi.plugin"

namespace sushi {

SharedLibraryRuntimePlugin::SharedLibraryRuntimePlugin(const char* lib_path) noexcept
: RuntimePlugin()
, library_path{lib_path}
, library_object{nullptr}
, start_fn{}
, stop_fn{}
, serialize_fn{}
, unserialize_fn{}
, get_api_fn{}
, serialization_zone{nullptr}
, serialization_zone_size{0} {

}

bool SharedLibraryRuntimePlugin::can_serialize() const noexcept {
    return serialize_fn != nullptr && unserialize_fn != nullptr;
}

int SharedLibraryRuntimePlugin::start() {
    if (load_functions()) {
        // Call plugin_start
        int result = start_fn();

        // if plugin_start failed
        if (result != 0) {
            log_warning(LOG_CATEGORY, "failed to initialize plugin '%s'", library_path);
            return result;
        }

        // Unserialize plugin
        if (can_serialize()) {
            unserialize_fn(serialization_zone, serialization_zone_size);
        }

        return RuntimePlugin::start();
    }

    return PLUGIN_FAILED;
}

void SharedLibraryRuntimePlugin::stop() {
    // Serialize plugin before stopping it
    if (can_serialize()) {
        serialization_zone_size = serialize_fn(&serialization_zone);

        // an error happened while serializing plugin
        if (serialization_zone_size < 0) {
            // Reset the serialization zone
            serialization_zone_size = 0;
            serialization_zone = nullptr;
            log_warning(LOG_CATEGORY, "failed to serialize plugin '%s'", library_path);
        }
    }

    // Call plugin's stop function
    stop_fn();

    // Close the library
    if (dlclose(library_object) != 0) {
        log_warning(LOG_CATEGORY, "failed to close shared library '%s': '%s", library_path, dlerror());
    }

    RuntimePlugin::stop();
}

ApiHandle SharedLibraryRuntimePlugin::get_api(int version) {
    return get_api_fn(version);
}

bool SharedLibraryRuntimePlugin::load_functions() {
    library_object = dlopen(library_path, RTLD_NOW | RTLD_LOCAL);

    if (library_object != nullptr) {
        start_fn = reinterpret_cast<plugin_start_fn>(dlsym(library_object, "plugin_start"));
        if (start_fn == nullptr) {
            log_critical(LOG_CATEGORY, "plugin '%s' is missing mandatory function 'plugin_start'", library_path);
        }

        stop_fn = reinterpret_cast<plugin_stop_fn>(dlsym(library_object, "plugin_stop"));
        if (stop_fn == nullptr) {
            log_critical(LOG_CATEGORY, "plugin '%s' is missing mandatory function 'plugin_stop'", library_path);
        }

        serialize_fn = reinterpret_cast<plugin_serialize_fn>(dlsym(library_object, "plugin_serialize"));
        unserialize_fn = reinterpret_cast<plugin_unserialize_fn>(dlsym(library_object, "plugin_unserialize"));

        get_api_fn = reinterpret_cast<plugin_get_api_fn>(dlsym(library_object, "plugin_get_api"));
        if (get_api_fn == nullptr) {
            log_critical(LOG_CATEGORY, "plugin '%s' is missing mandatory function 'plugin_get_api'", library_path);
        }

        if ((serialize_fn != nullptr && unserialize_fn == nullptr)
            || (serialize_fn == nullptr && unserialize_fn != nullptr)) {
            log_warning(LOG_CATEGORY,
                        "plugin '%s' is not defining both serialization functions: plugin_serialize and plugin_unserialize. Serialization will be disabled",
                        library_path);
        }

        return start_fn && stop_fn && get_api_fn;
    }
    else {
        log_critical(LOG_CATEGORY, "failed to load shared library '%s': %s", library_path, dlerror());
    }

    return false;
}

}

#undef LOG_CATEGORY