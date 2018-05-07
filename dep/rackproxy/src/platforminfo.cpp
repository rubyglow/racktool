// Platform and program version specific info

#include "platforminfo.hpp"
#include "fsnames.hpp"

#include <jansson.h>

// Serialize various version and platform info
std::string PlatformInfo::serialize(FsNames *fsNames) {
	json_t *info = json_object();
	
	json_object_set_new(info, "rackApiLevel", json_string(apiLevel.c_str()));
	json_object_set_new(info, "proxyVersion", json_string(appVersion.c_str()));
	json_object_set_new(info, "os", json_string(os.c_str()));
	json_object_set_new(info, "stdRackDir", json_string(fsNames->stdRackDir.c_str()));
	json_object_set_new(info, "stdPluginDir", json_string(fsNames->stdPluginDir.c_str()));
	json_object_set_new(info, "stdLibName", json_string(fsNames->stdLibName.c_str()));
	
	return json_dumps(info, 0);
}
