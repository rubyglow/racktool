// Load and serialize one plugin

#include "plugin.hpp"

#include <string>

using namespace rack;

// Wrapper for one Rack plugin
struct PluginWrapper {
	Plugin *plugin;
	std::string pluginDir;
	std::string pluginFile;
	std::string loadError;
	
	// Load the plugin in the directory
	bool load(std::string directory);

	// Serialize the loaded plugin
	std::string serialize();

	// Free resources
	void destroy();
};
