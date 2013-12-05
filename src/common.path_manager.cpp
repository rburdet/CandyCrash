#include "common.path_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>

#define MAX_PATH_SIZE 255

using std::string;

std::string PathManager::exePath;

PathManager::PathManager(){}
PathManager::~PathManager(){}

void PathManager::init(){
	PathManager::exePath = PathManager::getExePath();
}

void PathManager::destroy(){
}

string PathManager::getExePath(){
	char buf[MAX_PATH_SIZE];
	char linkname[64]; /* /proc/<pid>/exe */
	pid_t pid;
	int ret;
	string path = "";

	/* Get our PID and build the name of the link in /proc */
	pid = getpid();

	if (snprintf(linkname, sizeof(linkname), "/proc/%i/exe", pid) < 0){
		/* This should only happen on large word systems. I'm not sure
		   what the proper response is here.
		   Since it really is an assert-like condition, aborting the
		   program seems to be in order. */
		return path;
	}


	/* Now read the symbolic link */
	ret = readlink(linkname, buf, MAX_PATH_SIZE);

	/* In case of an error, leave the handling up to the caller */
	if (ret == -1)
		return NULL;

	/* Report insufficient buffer size */
	if (ret >= MAX_PATH_SIZE) {
		errno = ERANGE;
		return path;
	}

	/* Ensure proper NUL termination */
	buf[ret] = 0;

	path = dirname(buf);

	return path;
}

string PathManager::getPath(const string& str){
	return PathManager::getPath(str.c_str());

}
string PathManager::getPath(const char* str){
	string retPath = PathManager::exePath + "/" + str;
	return retPath;
}


int PathManager::chdir(){
	return ::chdir(PathManager::exePath.c_str());
}
