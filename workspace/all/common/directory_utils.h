/**
 * directory_utils.h - Directory utility functions
 *
 * Simple directory operations for checking directory contents.
 * Extracted from minui.c for testability.
 */

#ifndef __DIRECTORY_UTILS_H__
#define __DIRECTORY_UTILS_H__

/**
 * Checks if a directory contains any non-hidden files.
 *
 * Hidden files/directories start with '.' or are named ".." or ".".
 *
 * @param dir_path Full path to directory
 * @return 1 if directory exists and contains non-hidden files, 0 otherwise
 *
 * Example:
 *   hasNonHiddenFiles("/mnt/SDCARD/.minui/Collections") → 1 if has .txt files
 *   hasNonHiddenFiles("/empty/dir") → 0
 */
int Directory_hasNonHiddenFiles(const char* dir_path);

#endif // __DIRECTORY_UTILS_H__
