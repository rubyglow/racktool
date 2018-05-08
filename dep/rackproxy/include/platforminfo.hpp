// Platform and program version specific info

#include "fsnames.hpp"
#include "util/common.hpp"

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
	std::string serialize(FsNames *fsNames);
};
