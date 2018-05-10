// Load and serialize one plugin

#include "plugin.hpp"

#include <string>
#include <jansson.h>

using namespace rack;

// Wrapper for one Rack plugin
struct PluginWrapper {
	public:

	// Include this attempted plugin load in serialization?
	bool include = true;
	
	// JSON object serialization of plugin
	json_t *pluginJson;
	
	// Load the plugin in the directory
	bool load(std::string directory);

	// Serialize the loaded plugin to a JSON string
	std::string serialize();
	
	// Create and store the object serialization
	void createSerialization();

	// Free resources
	void destroy();

	private:

	Plugin *plugin;
	std::string pluginDir;
	std::string pluginFile;
	std::string loadError;
	bool serialized = false;
	bool loaded = false;

	// Load the builtin Core plugin	
	bool loadCore();
};
