# Plugins

## Supported types
Two types of plugins are supported:
1. Runtime plugins are modules loaded at run-time. They extend the engine by offering additional services.
2. Editor plugins are Java plugins extending the editor

## Runtime Plugin

### Lifetime ###
+-------------------------+
|       INACTIVE          |˂⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻+
+-------------------------+                       |
           |                                      |
           | Call to plugin_start                 |
           ˅                                      |
+-------------------------+                       |
|       STARTED           |                       |
+-------------------------+                       |
           |                                      |
           | Call to plugin_unserialize           |
           ˅                                      |
+-------------------------+                       | Call to plugin_stop
|      INITIALIZED        |                       |
+-------------------------+                       |
           |                                      |
           | Call to plugin_get_api               |
           ˅                                      |
+-------------------------+                       |
|      EXPOSED            |                       |
+-------------------------+                       |
           |                                      |
           | Call to plugin_serialize             |
           ˅                                      |
+-------------------------+                       |
|      SERIALIZED         |                       |
+-------------------------+                       |
           |                                      |
           +⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻⎻+

### Required exposed functions
```
int plugin_start()
```
[MANDATORY]
Initializes the plugin.
Should return 0 when the initialization succeeded or an error code otherwise.
A plugin that fails to initialize and that is marked as optional won't be loaded.
If the plugin is marked as required and fails to load, then the game will stop immediatly.

```
int plugin_serialize(void* serialization_zone)
```
[OPTIONAL]
Let the plugin serialize it's state in memory before shutting down. (This is there to allow hot reloading)
Should return the serialization used space in bytes
Should modify destination

```
int plugin_unserialize(const void* serialization_zone)
```
[OPTIONAL]
Let the plugin load a previously serialized state
Should return 0 on success or an error code

```
void plugin_stop()
```
[MANDATORY]
Let the plugin releases all it's ressource before being inactive.

```
void* plugin_get_api(int version);
```

### Manifest
A plugin must be accompanied by a plugin manifest. The file allow the plugin to expose it's services and it's requirements

The manifest is a XML file
```
<?xml version="1.0" encoding="utf-8" ?>
<plugin id="com.sushi.plugin"
        version="0.0.0">
   <!-- List the requirements to use this plugin -->
    <requirements>
        <!-- Indicate the engine version supported by this plugin -->
        <engine version="1.*" />
        <plugins>
            <plugin id="com.sushi.other_plugin" version="1.*.*"/>
        </plugins>
    </requirements>

    <!-- List all the capabilities of the plugin -->
    <capabilities>
        <!-- Can this plugin be hot reloaded -->
        <support name="hot_reloading" value="yes" />

        <!-- Supported operating systems -->
        <support name="windows" value="yes" />
        <support name="linux" value="yes" />
        <support name="macos" value="yes" />
    </capabilities>
</plugin>
```