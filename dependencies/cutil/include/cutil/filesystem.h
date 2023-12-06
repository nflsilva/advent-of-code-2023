#ifndef _CUTIL_FILESYSTEM_H
#define _CUTIL_FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>

#define CUTIL_FS_CHUNK_SIZE 1024

/*!
 * Loads into memory the contents of a file.
 * @param filePath The path to the asset to load
 * @return The data from the file, in bytes. `NULL` if the file doesn't exist.
 */
const char* cutilFilesystemLoadFile(const char* filePath);

#endif