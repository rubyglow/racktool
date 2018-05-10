// Common utilities

#include "util.hpp"
#include "platforminfo.hpp"

#include <sys/stat.h>
#include <dirent.h>

bool systemIsFile(std::string path) {
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf))
		return false;
	return S_ISREG(statbuf.st_mode);
}

bool systemIsDirectory(std::string path) {
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf))
		return false;
	return S_ISDIR(statbuf.st_mode);
}

std::vector<std::string> systemListEntries(std::string path) {
	std::vector<std::string> filenames;
	DIR *dir = opendir(path.c_str());
	if (dir) {
		struct dirent *d;
		while ((d = readdir(dir))) {
			std::string filename = d->d_name;
			if (filename == "." || filename == "..")
				continue;
			filenames.push_back(path + PATHSEP + filename);
		}
		closedir(dir);
	}
	return filenames;
}
