// Load and serialize one or more plugins

#include "pluginwrapper.hpp"

#include <string>
#include <list>

// Wrapper for a collection of Rack plugins
struct Plugins {
	public:
	
	// Load all plugins under directory (recursively, like Rack does)
	void load(std::string directory);
	
	// Serialize all loaded plugins to a JSON string
	std::string serialize();

	// Free resources
	void destroy();

	private:

	std::list<PluginWrapper*> pluginList;
	std::string baseDir;
	int numLoaded = 0;
	int numErrors = 0;

	// Load one plugin	
	void loadPlugin(std::string directory);
};
