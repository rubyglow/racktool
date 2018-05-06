// Operating system dependent standard filesystem names defined by Rack

#include <string>

struct FsNames {
	std::string stdRackDir;
	std::string stdPluginDir;
	std::string stdLibName;
};

// Get the standard filesystem names
FsNames *getFsNames();
