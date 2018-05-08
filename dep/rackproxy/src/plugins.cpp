// Load and serialize one or more plugins

#include "plugins.hpp"

#include "util/common.hpp"
	
// Load all plugins under directory (recursively in one level, like Rack does)
// Also load the builtin Core plugin
void Plugins::load(std::string directory) {
	baseDir = directory;
	printf("Loading plugins under %s\n", baseDir.c_str());

	// Load Core plugin
	loadPlugin("core");

	// Load plugins recursively
	for (std::string pluginDir : systemListEntries(baseDir)) {
		if (systemIsDirectory(pluginDir)) {
			loadPlugin(pluginDir);
		}
	}

	printf("Loaded %d plugins\n", numLoaded);
	printf("%d plugins could not be loaded\n", numErrors);
	return;
}

// Load one plugin
void Plugins::loadPlugin(std::string pluginDir) {
	bool loadSuccess;
	auto *plugin = new PluginWrapper();
	pluginList.push_back(plugin);
	loadSuccess = plugin->load(pluginDir);
	if(loadSuccess) numLoaded += 1; else numErrors += 1;
}

// Serialize all loaded plugins
std::string Plugins::serialize() {
	return "json here";
}

// Unload all plugins and free resources. Only call once!
void Plugins::destroy() {
	pluginList.clear();
	pluginDestroy();
}
