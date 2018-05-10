// Load and serialize one plugin

#include "pluginwrapper.hpp"
#include "platforminfo.hpp"
#include "util.hpp"

#if ARCH_WIN
	#include <direct.h>
#else
	#include <dlfcn.h>
#endif
	
std::string stdLibName = FsNames::get()->stdLibName;

// Load the plugin in the directory
bool PluginWrapper::load(std::string directory) {
	if(directory == "core") return loadCore();
	pluginDir = directory;
	pluginFile = pluginDir + PATHSEP + stdLibName;

	// Directory does not exist or does not contain a plugin file
	// Just ignore entirely - it never happened
	if (!systemIsFile(pluginFile)) {
		include = false;
		return false;
	}

	// Load dynamic/shared library
#if ARCH_WIN
	SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);
	HINSTANCE handle = LoadLibrary(pluginFile.c_str());
	SetErrorMode(0);
	if (!handle) {
		int error = GetLastError();
		loadError = "Failed to load library " + pluginFile + " (code " + error + ")";
		return false;
	}
#else
	void *handle = dlopen(pluginFile.c_str(), RTLD_NOW);
	if (!handle) {
		loadError = "Failed to load library " + pluginFile + " : " + dlerror();
		return false;
	}
#endif
	
	// Get handle for the plugin's init() function
	typedef void (*InitCallback)(Plugin *);
	InitCallback initCallback;
#if ARCH_WIN
	initCallback = (InitCallback) GetProcAddress(handle, "init");
#else
	initCallback = (InitCallback) dlsym(handle, "init");
#endif
	if (!initCallback) {
		loadError = "Failed to read init() symbol in " + pluginFile;
		return false;
	}

	// Construct and initialize Plugin instance
	plugin = new Plugin();
	plugin->path = pluginDir;
	plugin->handle = handle;
	initCallback(plugin);

	// Reject plugin if slug already exists
	Plugin *oldPlugin = pluginGetPlugin(plugin->slug);
	if (oldPlugin) {
		loadError = "Plugin " + plugin->slug + " is already loaded, not attempting to load it again.";
		// TODO
		// Fix memory leak with `plugin` here
		return false;
	}

	// Add plugin to list
	gPlugins.push_back(plugin);
	loaded = true;

	return true;
}

// Load the builtin Core plugin
bool PluginWrapper::loadCore() {
	plugin = new Plugin();
	init(plugin);

	// Add plugin to list
	gPlugins.push_back(plugin);
	loaded = true;

	return true;
}

// Serialize the loaded plugin and return as string
char* PluginWrapper::serialize() {
	if(!serialized) createSerialization();
	return json_dumps(pluginJson, 0);
}

// Create and store the object serialization of the plugin
void PluginWrapper::createSerialization() {
	pluginJson = json_object();
	json_t *modulesJson = json_array();
	json_t *moduleJson;
	json_t *tagsJson;
	
	json_object_set_new(pluginJson, "dir", json_string(pluginDir.c_str()));
	json_object_set_new(pluginJson, "file", json_string(pluginFile.c_str()));
	json_object_set_new(pluginJson, "loaded", json_boolean(loaded));
	json_object_set_new(pluginJson, "loadError", json_string(loadError.c_str()));

	// If there were errors loading the plugin we can't do any more
	if(!loaded) {
		serialized = true;
		return;
	}
	
	json_object_set_new(pluginJson, "slug", json_string(plugin->slug.c_str()));
	json_object_set_new(pluginJson, "version", json_string(plugin->version.c_str()));
	json_object_set_new(pluginJson, "numModules", json_integer(plugin->models.size()));
	
	// Modules in the plugin
	for (Model *model : plugin->models) {
		moduleJson = json_object();
		
		json_object_set_new(moduleJson, "slug", json_string(model->slug.c_str()));
		json_object_set_new(moduleJson, "name", json_string(model->name.c_str()));
		json_object_set_new(moduleJson, "author", json_string(model->author.c_str()));
		
		// Tags in the module
		tagsJson = json_array();
		for (ModelTag tag : model->tags) {
			json_array_append(tagsJson, json_string(gTagNames[tag].c_str()));
		}

		json_object_set_new(moduleJson, "tags", tagsJson);	
		json_array_append(modulesJson, moduleJson);
	}

	json_object_set_new(pluginJson, "modules", modulesJson);
	
	serialized = true;
}

// Unload plugin and free resources. Only call once!
void PluginWrapper::destroy() {
	pluginDestroy();
}
