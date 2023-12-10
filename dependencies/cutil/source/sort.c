#include "cutil/sort.h"

void cutilSortMergeSortMerge(void* data, int typeSize, int left, int mid, int right, int(*compare)(const void*, const void*)) 
{  
    int leftLength = mid - left + 1; 
    int rightLength = right - mid;

    void* L = malloc(leftLength * typeSize);
    void* R = malloc(rightLength * typeSize);
  
    memcpy(L, (char*)data + left * typeSize, leftLength * typeSize);
    memcpy(R, (char*)data + (mid + 1) * typeSize, rightLength * typeSize);

    int i = 0, j = 0; 
    int k = left; 

    while (i < leftLength && j < rightLength) {
        if (compare((char*)L + i * typeSize, (char*)R + j * typeSize) <= 0) {
            memcpy((char*)data + k * typeSize, (char*)L + i * typeSize, typeSize);
            i++;
        } else {
            memcpy((char*)data + k * typeSize, (char*)R + j * typeSize, typeSize);
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < leftLength) {
        memcpy((char*)data + k * typeSize, (char*)L + i * typeSize, typeSize);
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < rightLength) {
        memcpy((char*)data + k * typeSize, (char*)R + j * typeSize, typeSize);
        j++;
        k++;
    }

    // Free temporary arrays
    free(L);
    free(R);
} 

void cutilSortMergeSortSplit(void* data, int typeSize, int left, int right, int(*compare)(const void*, const void*)) 
{ 
    if (left < right) { 
        int mid = left + (right - left) / 2; 
        cutilSortMergeSortSplit(data, typeSize, left, mid, compare); 
        cutilSortMergeSortSplit(data, typeSize, mid + 1, right, compare); 
        cutilSortMergeSortMerge(data, typeSize, left, mid, right, compare); 
    } 
} 

void cutilSortMergeSort(void* data, int typeSize, int length, int(*compare)(const void*, const void*))
{
    cutilSortMergeSortSplit(data, typeSize, 0, length - 1, compare);
}