#ifndef SPOOKYSUSHI_SHARED_LIBRARY_RUNTIME_PLUGIN_HPP
#define SPOOKYSUSHI_SHARED_LIBRARY_RUNTIME_PLUGIN_HPP

#include "runtime_plugin.hpp"
#include <type_traits>

namespace sushi {

using plugin_start_fn = std::add_pointer<int()>::type;
using plugin_stop_fn = std::add_pointer<void()>::type;
using plugin_serialize_fn = std::add_pointer<int(void**)>::type;
using plugin_unserialize_fn = std::add_pointer<void(void*, int)>::type;
using plugin_get_api_fn = std::add_pointer<void*(int)>::type;

class SharedLibraryRuntimePlugin : public RuntimePlugin {
    const char* library_path;
    void* library_object;
    plugin_start_fn start_fn;
    plugin_stop_fn stop_fn;
    plugin_serialize_fn serialize_fn;
    plugin_unserialize_fn unserialize_fn;
    plugin_get_api_fn get_api_fn;
    void* serialization_zone;
    int serialization_zone_size;

    bool load_functions();
public:
    explicit SharedLibraryRuntimePlugin(const char* lib_path) noexcept;

    int start() override;
    void stop() override;
    ApiHandle get_api(int version) override;

    bool can_serialize() const noexcept;

};

}

#endif //SPOOKYSUSHI_SHARED_LIBRARY_RUNTIME_PLUGIN_HPP
