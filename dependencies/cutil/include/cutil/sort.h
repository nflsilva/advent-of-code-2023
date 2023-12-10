#ifndef _CUTIL_SORT_H
#define _CUTIL_SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cutilSortMergeSort(void* data, int typeSize, int length, int(*compare)(const void*, const void*));

#endif