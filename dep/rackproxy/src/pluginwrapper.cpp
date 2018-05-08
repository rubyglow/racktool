// Load and serialize one plugin

#include "pluginwrapper.hpp"
#include "fsnames.hpp"

#include "util/common.hpp"

#if ARCH_WIN
	#include <direct.h>
#else
	#include <dlfcn.h>
#endif

std::string stdLibName = getFsNames()->stdLibName;
	
// Load the plugin in the directory
bool PluginWrapper::load(std::string directory) {
	pluginDir = directory;
	pluginFile = pluginDir + "/" + stdLibName;
	printf("Loading plugin %s\n", pluginFile.c_str());

	// Check file existence
	if (!systemIsFile(pluginFile)) {
		printf("Plugin file %s does not exist\n", pluginFile.c_str());
		return false;
	}

	// Load dynamic/shared library
#if ARCH_WIN
	SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);
	HINSTANCE handle = LoadLibrary(pluginFile.c_str());
	SetErrorMode(0);
	if (!handle) {
		int error = GetLastError();
		printf("Failed to load library %s: code %d\n", pluginFile.c_str(), error);
		return false;
	}
#else
	void *handle = dlopen(pluginFile.c_str(), RTLD_NOW);
	if (!handle) {
		printf("Failed to load library %s:\n%s\n", pluginFile.c_str(), dlerror());
		return false;
	}
#endif
	
	// Call plugin's init() function
	typedef void (*InitCallback)(Plugin *);
	InitCallback initCallback;
#if ARCH_WIN
	initCallback = (InitCallback) GetProcAddress(handle, "init");
#else
	initCallback = (InitCallback) dlsym(handle, "init");
#endif
	if (!initCallback) {
		printf("Failed to read init() symbol in %s\n", pluginFile.c_str());
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
		printf("Plugin \"%s\" is already loaded, not attempting to load it again\n", plugin->slug.c_str());
		// TODO
		// Fix memory leak with `plugin` here
		return false;
	}

	// Add plugin to list
	gPlugins.push_back(plugin);

	return true;
}

// Serialize the loaded plugin
std::string PluginWrapper::serialize() {
	return "json here";
}

// Unload plugin and free resources. Only call once!
void PluginWrapper::destroy() {
	pluginDestroy();
}
