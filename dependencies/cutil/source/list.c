#include "cutil/list.h"

CUTILList* cutilListCreate()
{
    CUTILList* newList = (CUTILList*) malloc(sizeof(CUTILList));
    newList->size = 0;
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

void cutilListDestroy(CUTILList* list, int cleanData)
{
    if(!list) return;

    CUTILListNode* currentNode = list->head;
    while (currentNode != NULL) 
    {
        CUTILListNode* nextNode = currentNode->next;
        if(cleanData)
            free(currentNode->data);
        free(currentNode);
        currentNode = nextNode;
    }
    free(list);
}

void cutilListAppendElement(CUTILList* list, void* data)
{
    if(!list) return;

    CUTILListNode* newNode = (CUTILListNode*)malloc(sizeof(CUTILListNode));
    newNode->data = data;
    newNode->next = NULL;
    if(list->size == 0) 
        list->head = newNode;
    else
        list->tail->next = newNode;
    list->size++;
    list->tail = newNode;
}

void* cutilListGetElement(CUTILList* list, int index) 
{
    if(!list || list->size == 0 || index < 0 || index >= list->size ) return NULL;

    CUTILListNode* selectedElement = list->head;
    for(int i = 0; i != index; i++) 
        selectedElement = selectedElement->next;
    return selectedElement->data;
}

void* cutilListRemoveElement(CUTILList* list, int index)
{
    if(!list || list->size == 0 || index < 0 || index >= list->size ) return NULL;

    if(index == 0) return cutilListPopElement(list);

    CUTILListNode* currentElement = list->head;
    for(int i = 0; i != index - 1; i++) 
        currentElement = currentElement->next;

    CUTILListNode* removingElement = currentElement->next;
    currentElement->next = removingElement->next;

    void* dataPtr = removingElement->data;
    free(removingElement);
    list->size--;
    return dataPtr;
}

void cutilListPushElement(CUTILList* list, void* data)
{
    if(!list) return;

    CUTILListNode* newNode = (CUTILListNode*)malloc(sizeof(CUTILListNode));
    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;
    if(list->size == 0)
        list->tail = newNode;
    list->size++;
}

void* cutilListPopElement(CUTILList* list) 
{
    if(!list || list->size == 0) return NULL;

    CUTILListNode* removingElement = list->head;
    list->head = removingElement->next;

    void* dataPtr = removingElement->data;
    free(removingElement);
    list->size--;
    return dataPtr;
}