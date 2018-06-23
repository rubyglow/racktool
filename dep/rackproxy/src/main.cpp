// rackproxy - low level serialization of Rack and plugin metadata
// For use by a higher-level program

#include "platforminfo.hpp"
#include "plugins.hpp"
#include "moduletags.hpp"

#include <string.h>

#define BLURB "rackproxy %s (API level %s, Rack version %s, OS: %s)\n"

#define USAGE \
"For use by a higher level program\n" \
"Usage: rackproxy COMMAND [DIRECTORY]\n" \
"\n" \
"Commands:\n" \
"\n" \
"version              Show the rackproxy version and exit\n" \
"help                 Show this help text and exit\n" \
"platform             Get basic program and Rack defined info as JSON\n" \
"tags                 Get the Rack defined module tags as JSON\n" \
"plugins DIRECTORY    Get meta data about the plugins under DIRECTORY (recursively) as JSON\n" \
"plugin DIRECTORY     Get meta data about the plugin in DIRECTORY (or 'core') as JSON\n"

// Print version info
void printBlurb() {
	auto *platformInfo = new PlatformInfo();
	printf(BLURB, platformInfo->appVersion.c_str(), platformInfo->apiLevel.c_str(),
		platformInfo->rackVersion.c_str(), platformInfo->os.c_str());
}

int main(int argc, char* argv[]) {
	// Wrong number of arguments	
	if(argc < 2 || argc > 3) {
		printBlurb();
		printf(USAGE);
		return 1;
	}

	// Print version info and exit
	else if(!strcmp(argv[1], "version") && argc == 2) {
		printBlurb();
	}

	// Print version info + usage and exit
	else if(!strcmp(argv[1], "help") && argc == 2) {
		printBlurb();
		printf(USAGE);
	}

	// Print basic program and Rack defined info as JSON
	else if(!strcmp(argv[1], "platform") && argc == 2) {
		char *jsonStr = (new PlatformInfo())->serialize(); 
		printf("%s\n", jsonStr);
		free(jsonStr);
	}

	// Print Rack defined module tags as JSON
	else if(!strcmp(argv[1], "tags") && argc == 2) {
		tagsInit();
		auto *moduleTags = new ModuleTags();
		char *jsonStr = moduleTags->serialize();
		printf("%s\n", jsonStr);
		free(jsonStr);
	}

	// Print meta data about the plugins under a directory (recursively), including the Core plugin, as JSON.
	else if(!strcmp(argv[1], "plugins") && argc == 3) {
		tagsInit();
		auto *plugins = new Plugins();
		auto loadSuccess = plugins->load(argv[2]);
		if(loadSuccess) {
			char *jsonStr = plugins->serialize(); 
			printf("%s\n", jsonStr);
			free(jsonStr);
		}

		plugins->destroy();
	}

	// Print meta data about a plugin as JSON
	else if(!strcmp(argv[1], "plugin") && argc == 3) {
		tagsInit();
		auto *plugin = new PluginWrapper();
		plugin->load(argv[2]);
		if(plugin->include) {
			char *jsonStr = plugin->serialize();
			printf("%s\n", jsonStr);
			free(jsonStr);
		}

		plugin->destroy();
	}

	// Unrecognized or wrong number of arguments
	else {
		printBlurb();
		printf(USAGE);
		return 1;
	}

	return 0;
}
