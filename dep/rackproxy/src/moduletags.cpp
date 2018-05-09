// The Rack defined module tags

#include "moduletags.hpp"
#include "tags.hpp"

#include <jansson.h>

using namespace rack;

// Serialize all the module tags as a JSON string
std::string ModuleTags::serialize() {
	json_t *tagsJson = json_array();
	
	for (std::string tag : gTagNames) {
		json_array_append(tagsJson, json_string(tag.c_str()));
	}

	return json_dumps(tagsJson, 0);
}
