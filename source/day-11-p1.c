#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"
#include "cutil/list.h"

#define DEBUGa
#define MAX_GALAXIES 1024

typedef struct {
    int index;
    int id;
} Galaxy_t;

typedef struct {
    int* map;
    Galaxy_t* galaxies;
    int nGalaxies;
    int width, heigth; 
} Space_t;

typedef struct {
    int row, column, index;
    int distance;
}Node_t;

/**!
 * Auxiliar function to create a map node
 * @param row the node's row
 * @param column the node's column
 * @param distance the distance from the starting point
 * @return the newly created node
*/
Node_t* createNode(int row, int column, int index, int distance) {
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
int bfs(Space_t* space, int row, int column, int startGalaxyId)
{
    int result = 0;

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

            int targetIndex = targetRow * space->width + targetColumn;
            for(int j = 0; j < space->nGalaxies; j++) 
            {
                // only add the distance if
                // 1 - a galaxy was found
                // 2 - the galaxy was never reached before
                // 3 - this galaxy pair was never added to the final result
                if(
                    space->galaxies[j].index == targetIndex && 
                    space->galaxies[j].id > startGalaxyId && 
                    space->map[targetIndex] == 0) 
                {
                    result += currentNode->distance + 1;
                    //printf("%d -> %d : %d\n", startGalaxyId, space->galaxies[j].id, currentNode->distance + 1);
                }
            }

            // this node was never explored. add it to the back of the queue
            if(space->map[targetIndex] == 0)
            {
                cutilListAppendElement(queue, createNode(targetRow, targetColumn, targetIndex, currentNode->distance + 1));
                space->map[targetIndex] = 2;
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
                int index = r * space->width + c;
                char p = '.';
                if(space->map[index] == 1)
                    p = 'x';
                else if(space->map[index] == 2)
                    p = 'q';
                else 
                    for(int j = 0; j < space->nGalaxies; j++) 
                        if(space->galaxies[j].index == index)
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
    
    // read all file into
    const char* originalMap = cutilFilesystemLoadFile(filePath);
    if(!originalMap)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }
    
    // initialize context
    Space_t space;
    space.width = 0;
    space.heigth = 0;
    int mapLength = strlen(originalMap);
    
    // find map size and create auxiliar structures for counting galaxies
    while(originalMap[space.width] != '\n')
        space.width++;
    space.heigth = mapLength / space.width;
    space.nGalaxies = 0;
    int* nGalaxiesInColumn = (int*)malloc(sizeof(int) * space.width);
    int* nGalaxiesInRow = (int*)malloc(sizeof(int) * space.heigth);
    memset(nGalaxiesInColumn, 0, sizeof(int) * space.width);
    memset(nGalaxiesInRow, 0, sizeof(int) * space.heigth);

    // count how many galaxies are in each row / column
    for(int r = 0; r < space.heigth; r++)
    {
        for(int c = 0; c < space.width; c++)
        {
            int index = r * (space.width + 1) + c;
            if(originalMap[index] == '#')
            {
                nGalaxiesInColumn[c]++;
                nGalaxiesInRow[r]++;
            }
        }
    }

    // compute the new (expanded) map size by adding the extra (expanded) lines
    int nEmptyRows = space.heigth;
    int nEmptyColumns = space.width;
    for(int r = 0; r < space.heigth; r++)
        if(nGalaxiesInRow[r] != 0)
            nEmptyRows--;
    for(int c = 0; c < space.width; c++)
        if(nGalaxiesInColumn[c] != 0)
            nEmptyColumns--; 
    int expandedHeigth = space.heigth + nEmptyRows;
    int expandedWidth = space.width + nEmptyColumns;

    // create the expanded map
    // this is a brute force solution and isn't 
    // efficient for large maps
    space.galaxies = (Galaxy_t*)malloc(sizeof(Galaxy_t) * MAX_GALAXIES);
    space.map = (int*)malloc(sizeof(int) * expandedWidth * expandedHeigth);
    memset(space.map, 0, sizeof(int) * expandedWidth * expandedHeigth);

    // insert galaxies from old into new map
    // keep track of their array index
    int shiftedColumns = 0;
    int shiftedRows = 0;
    int galaxyId = 1;
    for(int r = 0; r < space.heigth; r++)
    {
        // found a fow to expand. shift all next columns
        if(nGalaxiesInRow[r] == 0) 
        {
            shiftedRows++;
            continue;
        }
        shiftedColumns = 0;
        for(int c = 0; c < space.width; c++)
        {

            // found a column to expand. shift all next columns
            if(nGalaxiesInColumn[c] == 0) 
            {
                shiftedColumns++;
                continue;
            }
            
            // map old galaxy on new expanded map
            int oldIndex = r * (space.width + 1) + c;
            if(originalMap[oldIndex] == '#')
            {
                int newIndex = (r + shiftedRows) * expandedWidth + (c + shiftedColumns);
                space.galaxies[space.nGalaxies].index = newIndex;
                space.galaxies[space.nGalaxies].id = galaxyId++;
                space.nGalaxies++;
            }
        }
    }
    // update next map size
    space.heigth = expandedHeigth;
    space.width = expandedWidth;

    // cleanup auxiliary structures
    free(nGalaxiesInColumn);
    free(nGalaxiesInRow);
    free((void*)originalMap);

    // start main loop. 
    // for each node, perform a Breadth-First Search (BFS) search algorithm 
    // find the distance shortest distance to all other nodes WHICH DIDN'T already find
    // their distance to the testing node.
    // so, starting in
    // 1 -> 2, 3, 4
    // 2 -> 3, 4
    // 3 -> 4 
    // so, only add the distance to nodes with id > node.id
    int totalResult = 0;
    for(int i = 0; i < space.nGalaxies; i++) 
    {
        int galaxyIndex = space.galaxies[i].index;
        int row = galaxyIndex / space.width;
        int column = galaxyIndex % space.width;
        totalResult += bfs(&space, row, column, space.galaxies[i].id);

        // reset the visited nodes
        memset(space.map, 0, sizeof(int) * expandedWidth * expandedHeigth);
    }

#ifdef DEBUG
    // debug
    for(int r = 0; r < space.heigth; r++)
    {
        for(int c = 0; c < space.width; c++)
        {
            int index = r * space.width + c;
            char p = '.';
            for(int k = 0; k < space.nGalaxies; k++) 
            {
                if(space.galaxies[k].index == index) {
                    p = '#';
                    break;
                }
            }

            printf("%c", p);
        }
        printf("\n");
    }
#endif
    // cleanup
    free(space.galaxies);
    free(space.map);

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}