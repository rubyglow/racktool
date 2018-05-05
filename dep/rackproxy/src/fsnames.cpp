// Standard filesystem names defined by Rack

#include "fsnames.hpp"

#include <string>

#if ARCH_MAC
#include <CoreFoundation/CoreFoundation.h>
#include <pwd.h>
#endif

#if ARCH_WIN
#include <Windows.h>
#include <Shlobj.h>
#endif

#if ARCH_LIN
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

static FsNames *fsNames;
static bool isInit = false; // Has the statically held copy been initialized?

// Get the standard names and initialize a static instance of FsNames
static void init() {
	fsNames = new FsNames();
	std::string homeDir;
	std::string docsDir;

#if ARCH_MAC
	// Get home directory
	struct passwd *pw = getpwuid(getuid());
	homeDir = pw->pw_dir;
	
	fsNames->stdRackDir = homeDir + "/Documents/Rack";
	fsNames->stdPluginDir = fsNames->stdRackDir + "/plugins";
#endif

#if ARCH_WIN
	// Get "My Documents" folder
	char buf[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, buf);
	docsDir = buf;
	
	fsNames->stdRackDir = docsDir + "/Rack";
	fsNames->stdPluginDir = fsNames->stdRackDir + "/plugins";
#endif

#if ARCH_LIN
	const char *home = getenv("HOME");
	if (!home) {
		struct passwd *pw = getpwuid(getuid());
		home = pw->pw_dir;
	}
	homeDir = home;
	
	fsNames->stdRackDir = homeDir + "/.Rack";
	fsNames->stdPluginDir = fsNames->stdRackDir + "/plugins";
#endif

	isInit = true;
}

// Return the standard names
FsNames *getFsNames() {
	if (!isInit) init();
	return fsNames;
}
