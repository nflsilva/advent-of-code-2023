#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"
#include "cutil/list.h"

#define DEBUGE
#define MAX_GALAXIES 1024
#define EXPAND_WEIGHT 1000000

typedef struct {
    int index;
    int id;
} Galaxy_t;

typedef struct {
    int* map;
    int* galaxyMap;
    Galaxy_t* galaxies;
    int nGalaxies;
    int width, heigth; 
    int* columnWeights;
    int* rowWeights;
} Space_t;

typedef struct {
    int row, column, index;
    long int distance;
} Node_t;

/**!
 * Auxiliar function to create a map node
 * @param row the node's row
 * @param column the node's column
 * @param distance the distance from the starting point
 * @return the newly created node
*/
Node_t* createNode(int row, int column, int index, long int distance) {
    Node_t* node = (Node_t*) malloc(sizeof(Node_t));
    node->row = row;
    node->column = column;
    node->index = index;
    node->distance = distance;
    return node;
}

/**!
 * Performs a Breadth-First Search (BFS) on a map of ints
 * @param space the problem context
 * @param row the starting row
 * @param column the starting column
 * @param startGalaxyId the id for the starting galaxy
 * @return the sum of the shortest distances between the starting galaxy and all the galaxies 
 * which id is biggen than the starting one
*/
long int bfs(Space_t* space, int row, int column, int startGalaxyId)
{
    long int result = 0;

    // all 4 different moviments: left, right, up, down
    int deltaRows[] =    {  0,  0, -1,  1 };
    int deltaColumns[] = { -1,  1,  0,  0 };

    // create a node queue and add the initial node
    int startIndex = row * space->width + column;
    CUTILList* queue = cutilListCreate();
    cutilListAppendElement(queue, createNode(row, column, startIndex, 0));

    while(queue->size != 0)
    {
        Node_t* currentNode = cutilListPopElement(queue);
        // check all 4 adjacent spots
        for(int i = 0; i < 4; i++) 
        {
            int targetRow = currentNode->row + deltaRows[i];
            int targetColumn = currentNode->column + deltaColumns[i];

            // make sure adjacent moviment is within map limits
            if(targetColumn < 0 || targetRow < 0 || targetColumn >= space->width || targetRow >= space->heigth) continue;

            // instead of expanding the memory it self, we add the weight 
            int movimentWeight = space->columnWeights[targetColumn];
            if(deltaRows[i] != 0)
                movimentWeight = space->rowWeights[targetRow];

            int mapIndex = targetRow * space->width + targetColumn;
            if(space->galaxyMap[mapIndex] != 0 && space->map[mapIndex] == 0)
            {
                int targetGalaxyId = space->galaxyMap[mapIndex];
                if(targetGalaxyId > startGalaxyId) {
                    result += currentNode->distance + movimentWeight;
#ifdef DEBUGA
                    printf("%d -> %d : %d\n", startGalaxyId, targetGalaxyId, currentNode->distance + movimentWeight);
#endif
                }
            }

            // this node was never explored. add it to the back of the queue
            if(space->map[mapIndex] == 0)
            {
                cutilListAppendElement(queue, createNode(targetRow, targetColumn, mapIndex, currentNode->distance + movimentWeight));
                space->map[mapIndex] = 2;
            }
        }
        // mark the current node as explored and free the memory
        space->map[currentNode->index] = 1;
        free(currentNode);

#ifdef DEBUG
        // debug
        for(int r = 0; r < space->heigth; r++)
        {
            for(int c = 0; c < space->width; c++)
            {
                int mapIndex = r * space->width + c;
                int galaxyIndex = r * space->width + c;
                char p = '.';
                if(space->map[mapIndex] == 1)
                    p = 'x';
                else if(space->map[mapIndex] == 2)
                    p = 'q';
                else if(space->galaxyMap[galaxyIndex] != 0)
                    p = '#';

                printf("%c", p);
            }
            printf("\n");
        }
        printf("\n\n");
#endif
    }

    // cleanup
    cutilListDestroy(queue, 1);
    return result;
}

int main(int argc, char** argv)
{
    
    if(argc < 2)
    {
        printf("No file provided");
        exit(EXIT_FAILURE);
    }
    char* filePath = argv[1];
    
    // read all file into memory
    const char* fileData = cutilFilesystemLoadFile(filePath);
    if(!fileData)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }
    
    // initialize context
    Space_t space;
    space.width = 0;
    space.heigth = 0;
    int mapLength = strlen(fileData);
    
    // find map size and create context structures
    // note: i could probably optimize the memory usage here. But I'm 
    // kinda falling behing these puzzles to I'm moving foward for now.
    while(fileData[space.width] != '\n')
        space.width++;
    space.heigth = mapLength / space.width;
    space.nGalaxies = 0;
    // instead of expanding the memory and to avoid iterating over millions of empty 
    // cells, we keep track of each row / column weight.
    // empty rows / columns will weigh more and regular ones
    space.columnWeights = (int*)malloc(sizeof(int) * space.width);
    space.rowWeights = (int*)malloc(sizeof(int) * space.heigth);
    space.galaxies = (Galaxy_t*)malloc(sizeof(Galaxy_t) * MAX_GALAXIES);
    space.map = (int*)malloc(sizeof(int) * space.width * space.heigth);
    space.galaxyMap = (int*)malloc(sizeof(int) * space.width * space.heigth);
    for(int i = 0; i < space.width; i++)
        space.columnWeights[i] = EXPAND_WEIGHT;
    for(int i = 0; i < space.heigth; i++)
        space.rowWeights[i] = EXPAND_WEIGHT;
    
    // find which rows have galaxies
    // set their weight to 1 if any galaxy is found
    for(int r = 0; r < space.heigth; r++)
    {
        for(int c = 0; c < space.width; c++)
        {
            int oldIndex = r * (space.width + 1) + c;
            int newIndex = r * space.width + c;
            if(fileData[oldIndex] == '#')
            {
                space.columnWeights[c] = 1;
                space.rowWeights[r] = 1;
                space.galaxies[space.nGalaxies].index = newIndex;
                space.galaxies[space.nGalaxies].id = space.nGalaxies + 1;
                space.galaxyMap[newIndex] = space.galaxies[space.nGalaxies].id;
                space.nGalaxies++;
            }
            else {
                space.galaxyMap[newIndex] = 0;
            }
        }
    }

    // no longer needed
    free((void*)fileData);

    // start main loop. 
    // for each node, perform a Breadth-First Search (BFS) search algorithm 
    // find the distance shortest distance to all other nodes WHICH DIDN'T already find
    // their distance to the testing node.
    // so, starting in
    // 1 -> 2, 3, 4
    // 2 -> 3, 4
    // 3 -> 4 
    // so, only add the distance to nodes with id > node.id
    long int totalResult = 0;
    for(int i = 0 ; i < space.nGalaxies; i++) 
    {
        // reset the visited nodes
        memset(space.map, 0, sizeof(int) * space.width * space.heigth);

        int galaxyIndex = space.galaxies[i].index;
        int row = galaxyIndex / space.width;
        int column = galaxyIndex % space.width;
        totalResult += bfs(&space, row, column, space.galaxies[i].id);
    }

#ifdef DEBUG
    // debug
    for(int r = 0; r < space.heigth; r++)
    {
        for(int c = 0; c < space.width; c++)
        {
            int mapIndex = r * space.width + c;
            char p = '.';
            if(space.galaxyMap[mapIndex] != '.')
                p = '#';
            printf("%c", p);
        }
        printf("\n");
    }
#endif
    // cleanup
    free(space.columnWeights);
    free(space.rowWeights);
    free(space.galaxies);
    free(space.map);
    free((void*)space.galaxyMap);

    printf("Result: %ld\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}