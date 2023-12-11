#include "cutil/hashTable.h"

// http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2Hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

CUTILHashTableBucketChainNode* createBucketChainNode(const char* key, void* data)
{
    CUTILHashTableBucketChainNode* newNode = (CUTILHashTableBucketChainNode*)malloc(sizeof(CUTILHashTableBucketChainNode));
    newNode->data = data;
    newNode->key = key;
    return newNode;
}

CUTILHashTableBucket* getBucketForKey(CUTILHashTable* hashTable, const char* key)
{
    int bucketIndex = djb2Hash(key) % _CUTIL_TABLE_SIZE;
    return &hashTable->buckets[bucketIndex];
}

CUTILHashTable* cutilHashTableCreate()
{
    CUTILHashTable* newTable = (CUTILHashTable*) malloc(sizeof(CUTILHashTable));
    newTable->buckets = (CUTILHashTableBucket*)malloc(sizeof(CUTILHashTableBucket) * _CUTIL_TABLE_SIZE);
    memset(newTable->buckets, 0, sizeof(CUTILHashTableBucket) * _CUTIL_TABLE_SIZE);
    return newTable;
}

void cutilHashTableDestroy(CUTILHashTable* hashTable, int cleanData)
{
    if(!hashTable) return;

    for(int i = 0; i < _CUTIL_TABLE_SIZE; i++) 
    {
        CUTILHashTableBucket* bucket = &hashTable->buckets[i];
        if(bucket->chain != NULL) 
            cutilListDestroy(bucket->chain, cleanData);
    }
    free(hashTable->buckets);
    free(hashTable);
}

void cutilHashTableAddElement(CUTILHashTable* hashTable, const char* key, void* data)
{
    if(!hashTable) return;

    CUTILHashTableBucket* bucket = getBucketForKey(hashTable, key);

    // bucket is unused
    if(bucket->chain == NULL)
    {
        bucket->chain = cutilListCreate();
    }
    else 
    {
        // search for this key on the chain and update the value if needed
        for(int i = 0; i < bucket->chain->size; i++)
        {
            CUTILHashTableBucketChainNode* node = cutilListGetElement(bucket->chain, i);
            if(strcmp(node->key, key) == 0) 
            {
                node->data = data;
                return;
            }
        }
    }

    // add the data to the chain
    cutilListAppendElement(bucket->chain, createBucketChainNode(key, data));    
}

void* cutilHashTableGetElement(CUTILHashTable* hashTable, const char* key) 
{
    if(!hashTable) return NULL;

    CUTILHashTableBucket* bucket = getBucketForKey(hashTable, key);

    // key doesn't exist
    if(bucket->chain == NULL || bucket->chain->size == 0) return NULL;

    // key has no collisions
    if(bucket->chain->size == 1) return ((CUTILHashTableBucketChainNode*)cutilListGetElement(bucket->chain, 0))->data;

    // find the key on chain
    for(int i = 0; i < bucket->chain->size; i++) 
    {
        CUTILHashTableBucketChainNode* node = cutilListGetElement(bucket->chain, i);
        if(strcmp(node->key, key) == 0) return node->data;
    }

    return NULL;
}

void* cutilHashTableRemoveElement(CUTILHashTable* hashTable, const char* key) 
{
    if(!hashTable) return NULL;

    CUTILHashTableBucket* bucket = getBucketForKey(hashTable, key);

    // key doesn't exist
    if(bucket->chain == NULL || bucket->chain->size == 0) return NULL;

    // check if this was the last element on the chain and updates bucket if so
    if(bucket->chain->size == 1) {
        CUTILHashTableBucketChainNode* node = cutilListGetElement(bucket->chain, 0);
        void* dataPtr = node->data;
        cutilListDestroy(bucket->chain, 0);
        bucket->chain = NULL;
        free(node);
        return dataPtr;
    }

    // find the key on chain
    void* dataPtr = NULL;
    for(int i = 0; i < bucket->chain->size; i++) 
    {
        CUTILHashTableBucketChainNode* node = cutilListGetElement(bucket->chain, i);
        if(strcmp(node->key, key) == 0) 
        {
            node = cutilListRemoveElement(bucket->chain, i);
            dataPtr = node->data;
            free(node);
            break;
        }
    }

    return dataPtr;
}