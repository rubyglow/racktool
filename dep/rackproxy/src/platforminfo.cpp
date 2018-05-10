// Platform, operating system and program version specific info

#include "platforminfo.hpp"

#include <jansson.h>

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

// Statically held copy of FsNames* that we dish out
// Just a bit of premature optimization :-)
static FsNames *fsNames;
static bool isInit = false;

// Serialize various version and platform info
char* PlatformInfo::serialize() {
	auto *fsNames = FsNames::get();
	auto *info = json_object();
	
	json_object_set_new(info, "proxyVersion", json_string(appVersion.c_str()));
	json_object_set_new(info, "rackApiLevel", json_string(apiLevel.c_str()));
	json_object_set_new(info, "rackVersion", json_string(rackVersion.c_str()));
	json_object_set_new(info, "os", json_string(os.c_str()));
	json_object_set_new(info, "stdRackDir", json_string(fsNames->stdRackDir.c_str()));
	json_object_set_new(info, "stdPluginDir", json_string(fsNames->stdPluginDir.c_str()));
	json_object_set_new(info, "stdLibName", json_string(fsNames->stdLibName.c_str()));
	
	return json_dumps(info, 0);
}

// Initialize operating system dependent names, once
static void initFsNames() {
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
	// Get home directory
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

// Get static instance of FsNames. Use this instead of new FsNames().
FsNames* FsNames::get() {
	if(!isInit) initFsNames();
	return fsNames;
}
