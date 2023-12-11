#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/hashTable.h"

typedef struct {
    int isFinal;
    char left[4];
    char right[4];
} Node_t;

Node_t* createMapNode(char name[], char left[], char right[])
{
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->isFinal = strcmp(name, "ZZZ") == 0;
    strcpy(node->left, left);
    strcpy(node->right, right);
    return node;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("No file provided");
        exit(EXIT_FAILURE);
    }
    char* filePath = argv[1];
    
    FILE* file = fopen(filePath, "r");
    if(!file)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    int totalResult = 0;
    
    // read moviment cicle from file
    char c;
    char moviments[1024] = { 0 };
    int nMoviments = 0;
    while((c = fgetc(file)) != '\n')
        moviments[nMoviments++] = c;

    // discard extra '\n'
    fgetc(file);

    // use hash table so lookup times are O(1)
    CUTILHashTable* map = cutilHashTableCreate();

    // create buffers for current node, left and right names.
    char currentNodeName[10] = { 0 };
    char leftName[10] = { 0 };
    char rightName[10] = { 0 };
    Node_t* allNodes[1024];
    int nNodes = 0;

    while(fscanf(file, "%s = (%s %s\n", &currentNodeName, &leftName, &rightName) != -1)
    {
        // make sure these strings are length are limited to 3.
        currentNodeName[3] = '\0';
        leftName[3] = '\0';
        rightName[3] = '\0';

        // save this node on an hash table so lookup times are O(1)
        Node_t* node = createMapNode(currentNodeName, leftName, rightName);
        cutilHashTableAddElement(map, currentNodeName, node);
        allNodes[nNodes++] = node;
    }

    // close file, as it is not needed anymore
    fclose(file);

    // follow the trail using the hashtable
    Node_t* currentNode = cutilHashTableGetElement(map, "AAA");
    int movimentsIndex = 0;
    while(!currentNode->isFinal)
    {
        // choose the appropriated path, based on moviments map
        if(moviments[movimentsIndex] == 'L')
            currentNode = cutilHashTableGetElement(map, currentNode->left);
        else 
            currentNode = cutilHashTableGetElement(map, currentNode->right);

        // make sure moviments repeat.
        movimentsIndex = ++movimentsIndex % nMoviments;
        totalResult++;
    }

    // cleanup
    cutilHashTableDestroy(map, 1);
    for(int i = 0; i < nNodes; i++)
        free(allNodes[i]);

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}