// Platform, operating system and program version specific info

#pragma once

#include "util.hpp"
#include <string>

struct PlatformInfo {
	const std::string appVersion = TOSTRING(VERSION);
	const std::string apiLevel = TOSTRING(API_LEVEL);
	const std::string rackVersion = TOSTRING(RACK_VERSION);

#if ARCH_MAC
	const std::string os = "mac";
#endif
#if ARCH_WIN
	const std::string os = "win";
#endif
#if ARCH_LIN
	const std::string os = "lin";
#endif

	// Serialize various version and platform info
	char* serialize();
};

struct FsNames {
	std::string stdRackDir;
	std::string stdPluginDir;

#if ARCH_MAC
	const std::string stdLibName = "plugin.dylib";
#endif
#if ARCH_WIN
	const std::string stdLibName = "plugin.dll";
#endif
#if ARCH_LIN
	const std::string stdLibName = "plugin.so";
#endif

	// Get static instance of FsNames. Use this instead of new FsNames().
	static FsNames* get();
};
