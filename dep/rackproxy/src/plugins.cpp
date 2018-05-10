// Load and serialize one or more plugins

#include "plugins.hpp"
#include "util/common.hpp"

#include <jansson.h>
	
// Load all plugins under directory (recursively in one level, like Rack does)
// Also load the builtin Core plugin
void Plugins::load(std::string directory) {
	baseDir = directory;

	// Load Core plugin
	loadPlugin("core");

	// Load plugins recursively
	for (std::string pluginDir : systemListEntries(baseDir)) {
		if (systemIsDirectory(pluginDir)) {
			loadPlugin(pluginDir);
		}
	}

	return;
}

// Load one plugin
void Plugins::loadPlugin(std::string pluginDir) {
	auto *plugin = new PluginWrapper();
	pluginList.push_back(plugin);
	auto loadSuccess = plugin->load(pluginDir);

	if(plugin->include) {
		if(loadSuccess) numLoaded += 1; else numErrors += 1;
	}
}

// Serialize all loaded plugins and return as string
std::string Plugins::serialize() {
	json_t *allJson = json_object();
	json_t *pluginsJson = json_array();

	// Top metadata	
	json_object_set_new(allJson, "baseDir", json_string(baseDir.c_str()));
	json_object_set_new(allJson, "numLoaded", json_integer(numLoaded));
	json_object_set_new(allJson, "numErrors", json_integer(numErrors));

	// Serialize all plugins
	for(PluginWrapper *plugin : pluginList) {
		if(plugin->include) {
			plugin->createSerialization();
			json_array_append(pluginsJson, plugin->pluginJson);
		}
	}
	
	json_object_set_new(allJson, "plugins", pluginsJson);
	
	return json_dumps(allJson, 0);
}

// Unload all plugins and free resources. Only call once!
void Plugins::destroy() {
	pluginList.clear();
	pluginDestroy();
}
