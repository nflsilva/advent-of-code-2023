#include <assert.h>
#include <stdlib.h>

#include "cutil/sort.h"

#define SORT_LENGTH 100

typedef struct {
    int parameter;
} Custom_t;

int customComparator(const void* a, const void* b) {
    return ((Custom_t*)a)->parameter - ((Custom_t*)b)->parameter;
}

int main() 
{
    // setup
    Custom_t itens[SORT_LENGTH];
    for(int i = 0; i < SORT_LENGTH; i++)
        itens[i].parameter = (SORT_LENGTH - i) % SORT_LENGTH;

    // sort
    cutilSortMergeSort(itens, sizeof(Custom_t), SORT_LENGTH, &customComparator);

    // check for errors
    Custom_t* currentItem = &itens[0];
    for(int i = 1; i < SORT_LENGTH; i++)
    {
        assert(currentItem->parameter < itens[i].parameter);
        currentItem = &itens[i];
    }
        
    exit(EXIT_SUCCESS);
}