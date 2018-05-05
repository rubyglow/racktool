// rackproxy - low level serialization of Rack and plugin metadata
// For use by a higher-level program

#include "fsnames.hpp"
#include "plugin.hpp"
#include "util/common.hpp"

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <list>
#include <dirent.h>

#if ARCH_WIN
	#include <direct.h>
#else
	#include <dlfcn.h>
#endif

#define BLURB "rackproxy %s (Rack API level %s)\n"

#define USAGE \
"For use by a higher level program\n" \
"Usage: rackproxy COMMAND [DIRECTORY]\n" \
"\n" \
"Commands:\n" \
"version:            Show the rackproxy version and exit\n" \
"help:               Show this help text and exit\n" \
"environment:        Get basic program version and Rack directory info as JSON\n" \
"plugins DIRECTORY:  Get meta data about the plugins in DIRECTORY (recursively) as JSON\n"

std::string appVersion = TOSTRING(VERSION);
std::string apiLevel = "0.6";

using namespace rack;

// Print plugin metadata to screen
static bool printPlugin(Plugin *plugin) {
	printf("Slug: %s\n", plugin->slug.c_str());
	printf("Version: %s\n", plugin->version.c_str());
	printf("\nModules:\n");
	
	// List modules
	for (Model *model : plugin->models) {
		printf("\n");
		printf("Author: %s\n", model->author.c_str());
		printf("Slug: %s\n", model->slug.c_str());
		printf("Name: %s\n", model->name.c_str());
		
		printf("Tags: ");
		for (ModelTag tag : model->tags) {
			printf("%s, ", gTagNames[tag].c_str());
		}
		printf("\n");
	}
	
	return true;
}

// Load one plugin
static bool loadPlug(std::string path) {
	std::string libraryFilename;
#if ARCH_LIN
	libraryFilename = path + "/" + "plugin.so";
#elif ARCH_WIN
	libraryFilename = path + "/" + "plugin.dll";
#elif ARCH_MAC
	libraryFilename = path + "/" + "plugin.dylib";
#endif

	// Check file existence
	if (!systemIsFile(libraryFilename)) {
		printf("Plugin file %s does not exist\n", libraryFilename.c_str());
		return false;
	}

	// Load dynamic/shared library
#if ARCH_WIN
	SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);
	HINSTANCE handle = LoadLibrary(libraryFilename.c_str());
	SetErrorMode(0);
	if (!handle) {
		int error = GetLastError();
		printf("Failed to load library %s: code %d\n", libraryFilename.c_str(), error);
		return false;
	}
#else
	void *handle = dlopen(libraryFilename.c_str(), RTLD_NOW);
	if (!handle) {
		printf("Failed to load library %s:\n%s\n", libraryFilename.c_str(), dlerror());
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
		printf("Failed to read init() symbol in %s\n", libraryFilename.c_str());
		return false;
	}

	// Construct and initialize Plugin instance
	Plugin *plugin = new Plugin();
	plugin->path = path;
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
	printf("Loaded plugin %s\n", libraryFilename.c_str());

	printPlugin(plugin);
	return true;
}

// Load all plugins
static bool loadAllPlugs() {
	// Get the standard Rack directories
	FsNames *fsNames = getFsNames();
	printf("Loading all plugins in your Rack plugins directory %s + core\n\n", fsNames->stdPluginDir.c_str());

	// Load the built-in core plugin
	Plugin *corePlugin = new Plugin();
	init(corePlugin);
	gPlugins.push_back(corePlugin);
	printf("Loaded built-in plugin Core\n");
	printPlugin(corePlugin);
	printf("\n");

	// Load all plugins
	for (std::string pluginPath : systemListEntries(fsNames->stdPluginDir)) {
		if (systemIsDirectory(pluginPath)) {
			loadPlug(pluginPath);
			printf("\n");
		}
	}

	return true;
}

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		printf(BLURB, appVersion.c_str(), apiLevel.c_str());
		printf(USAGE);
		return 1;
	}
	if(!strcmp(argv[1], "version") && argc == 2) {
		printf(BLURB, appVersion.c_str(), apiLevel.c_str());
		return 0;
	}
	else if(!strcmp(argv[1], "environment") && argc == 2) {
		printf("environment\n");
	}
	else if(!strcmp(argv[1], "plugins") && argc == 3) {
		tagsInit();
		loadAllPlugs();
		pluginDestroy();
	}
	else {
		printf(BLURB, appVersion.c_str(), apiLevel.c_str());
		printf(USAGE);
		return 1;
	}

	return 0;
}
