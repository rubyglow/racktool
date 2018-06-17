// Common utilities

#include "util.hpp"
#include "platforminfo.hpp"

#include <sys/stat.h>
#include <dirent.h>

#if ARCH_WIN
#include <Windows.h>
#include <strsafe.h>
#endif

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
			filenames.push_back(path + "/" + filename);
		}
		closedir(dir);
	}
	return filenames;
}

#if ARCH_WIN
// Get the last system error message from Windows.
// Because this is supposed to be very complicated, yikes!
std::string GetLastErrorMsg(int errCode) {
	std::string retval;
	LPVOID lpMsgBuf;
	DWORD dw = errCode;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		dw,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	retval = std::string((LPTSTR) lpMsgBuf);
	LocalFree(lpMsgBuf);
	
	return retval;
}
#endif
