// Standard filesystem names defined by Rack

#include <string>

struct FsNames {
	std::string stdRackDir;
	std::string stdPluginDir;
};

// Get the standard filesystem names
FsNames *getFsNames();
