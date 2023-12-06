#include <assert.h>
#include <stdlib.h>

#include "cutil/list.h"

#define LIST_SIZE 10

int main() 
{
    
    // setup elements array
    int elements[LIST_SIZE];
    for(int i = 0; i < LIST_SIZE; i++) 
        elements[i] = i;

    // setup list
    CUTILList* list = cutilListCreate();
    assert(list->size == 0);
    assert(list->head == NULL);
    for(int i = 0; i < LIST_SIZE; i++)
        cutilListAppendElement(list, &(elements[i]));

    // test initial state
    assert(list->size == LIST_SIZE);
    assert(list->head != NULL);
    assert(*(int*)list->head->data == 0);
    assert(*(int*)list->tail->data == LIST_SIZE - 1);

    // test remove
    assert(*(int*)cutilListRemoveElement(list, 4) == elements[4]);
    assert(*(int*)cutilListRemoveElement(list, 4) == elements[5]);
    assert(list->size == LIST_SIZE - 2);

    // test push / pop
    cutilListPushElement(list, &elements[5]);
    assert(*(int*)cutilListPopElement(list) == elements[5]);
    assert(list->size == LIST_SIZE - 2);

    // cleanup
    cutilListDestroy(list, 0);
    exit(EXIT_SUCCESS);
}