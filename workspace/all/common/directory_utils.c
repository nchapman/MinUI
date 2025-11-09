/**
 * directory_utils.c - Directory utility functions
 *
 * Extracted from minui.c for testability.
 */

#include "directory_utils.h"
#include "utils.h"
#include <dirent.h>
#include <stdio.h>

/**
 * Checks if a directory contains any non-hidden files.
 */
int Directory_hasNonHiddenFiles(const char* dir_path) {
	if (!exists((char*)dir_path)) {
		return 0;
	}

	DIR* dh = opendir(dir_path);
	if (!dh) {
		return 0;
	}

	struct dirent* dp;
	while ((dp = readdir(dh)) != NULL) {
		if (hide(dp->d_name)) {
			continue;
		}
		closedir(dh);
		return 1;
	}

	closedir(dh);
	return 0;
}
