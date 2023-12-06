#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct CUTILListNode {
    void* data;
    struct CUTILListNode* next;
} CUTILListNode;

typedef struct CUTILList {
    int size;
    CUTILListNode* head;
    CUTILListNode* tail;
} CUTILList;

/*!
 * Creates a new empty list.
 * @returns A pointer to a new list.
 */
CUTILList* cutilListCreate();

/*!
 * Destroy a list and its elements.
 * @param list A pointer to the list.
 * @param cleanData Set to anything other than `0` to free all the stored data. `0` otherwise.
 * @returns A pointer to the list to destroy.
 */
void cutilListDestroy(CUTILList* list, int cleanData);

/*!
 * Adds new element to the end of the list.
 * @param list A pointer to the list.
 * @param data A pointer to the data to keep in the list.
 */
void cutilListAppendElement(CUTILList* list, void* data);

/*!
 * Gets an element from the list.
 * @param list A pointer to the list.
 * @param index The list index of the element to retrive.
 * @return The data from the head element. `NULL` the list is empty.
 */
void* cutilListGetElement(CUTILList* list, int index);

/*!
 * Removes an element from the list.
 * @param list A pointer to the list.
 * @param index The list index of the element to remove.
 * @return The data from the head element. `NULL` the list is empty.
 */
void* cutilListRemoveElement(CUTILList* list, int index);

/*!
 * Adds new element to the start of the list.
 * @param list A pointer to the list.
 * @param data A pointer to the data to keep in the list.
 */
void cutilListPushElement(CUTILList* list, void* data);

/*!
 * Removes the first element from the list.
 * @param list A pointer to the list.
 * @return The data from the head element. `NULL` the list is empty.
 */
void* cutilListPopElement(CUTILList* list);

#endif