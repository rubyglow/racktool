// Load and serialize one or more plugins

#include "plugins.hpp"

#include "util/common.hpp"
	
// Load all plugins under directory (recursively in one level, like Rack does)
void Plugins::load(std::string directory) {
	baseDir = directory;
	bool loadSuccess;
	printf("Loading plugins under %s\n", baseDir.c_str());
	
	for (std::string pluginDir : systemListEntries(baseDir)) {
		if (systemIsDirectory(pluginDir)) {
			auto *plugin = new PluginWrapper();
			pluginList.push_back(plugin);
			loadSuccess = plugin->load(pluginDir);
			if(loadSuccess) numLoaded += 1; else numErrors += 1;
		}
	}

	printf("Loaded %d plugins\n", numLoaded);
	printf("%d plugins could not be loaded\n", numErrors);
	return;
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
