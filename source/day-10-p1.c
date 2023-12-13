#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"

int finalResult = 0;

typedef struct Node_t
{
    int row, column;
    int distance;
} Node_t;

typedef struct
{
    const char* map;
    int* distances;
    int width, heigth; 
} Maze_t;

int hasConnectionTo(Maze_t* maze, Node_t* currentNode, int deltaRow, int deltaColumn)
{
    // check if current node has a connection to destination
    int mazeIndex = currentNode->row * (maze->width + 1) + currentNode->column;
    char currentChar = maze->map[mazeIndex];
    if(currentChar == '|' && deltaColumn != 0) return 0;
    if(currentChar == '-' && deltaRow != 0) return 0;
    if(currentChar == 'L' && (deltaRow == 1 || deltaColumn == -1)) return 0;
    if(currentChar == 'J' && (deltaRow == 1 || deltaColumn == 1)) return 0;
    if(currentChar == '7' && (deltaRow == -1 || deltaColumn == 1)) return 0;
    if(currentChar == 'F' && (deltaRow == -1 || deltaColumn == -1)) return 0;

    // check if destination is within the maze limits
    int destinationRow = currentNode->row + deltaRow;
    int destinationColumn = currentNode->column + deltaColumn;
    if(destinationRow < 0 || destinationRow > maze->heigth || destinationColumn < 0 || destinationColumn > maze->width)
        return 0;

    // check if destination is unexplored
    if(maze->distances[destinationRow * maze->width + destinationColumn] != -1) return 0;

    // check if destination has a connection to current node
    char destinationChar = maze->map[destinationRow * (maze->width + 1) + destinationColumn];
    if(destinationChar == '.' || destinationChar == 'S') return 0;
    if(destinationChar == '|' && deltaColumn != 0) return 0;
    if(destinationChar == '-' && deltaRow != 0) return 0;
    if(destinationChar == 'L' && (deltaRow == -1 || deltaColumn == 1)) return 0;
    if(destinationChar == 'J' && (deltaRow == -1 || deltaColumn == -1)) return 0;
    if(destinationChar == '7' && (deltaRow == 1 || deltaColumn == -1)) return 0;
    if(destinationChar == 'F' && (deltaRow == 1 || deltaColumn == 1)) return 0;
    
    // all tests passed. Connection is possible
    return 1;
}

int moveNoteToNextAvailablePosition(Maze_t* maze, Node_t* currentNode)
{                       //left, right, up, down
    int deltaRows[] =    {  0,  0, -1,  1 };
    int deltaColumns[] = { -1,  1,  0,  0 };

    // check all 4 adjacent spots
    for(int i = 0; i < 4; i++) 
    {
        if(!hasConnectionTo(maze, currentNode, deltaRows[i], deltaColumns[i])) continue;

        // found a connection. move the current node and finish
        currentNode->column += deltaColumns[i];
        currentNode->row += deltaRows[i];
        currentNode->distance++;
        int mazeIndex = currentNode->row * maze->width + currentNode->column;
        maze->distances[mazeIndex] = currentNode->distance;
        if(currentNode->distance > finalResult)
            finalResult = currentNode->distance;
        return 1;
    }

    return 0;
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
    Maze_t maze;
    maze.map = cutilFilesystemLoadFile(filePath);
    if(!maze.map)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    // compute maze limits
    int fileLength = strlen(maze.map);
    maze.width = 0;
    while(maze.map[maze.width] != '\n')
        maze.width++;
    maze.heigth = fileLength / maze.width;
    maze.distances = (int*)malloc(sizeof(int) * maze.heigth * maze.width);
    memset(maze.distances, -1, sizeof(int) * maze.heigth * maze.width);

    // find S (start)
    int startIndex = 0;
    while(maze.map[startIndex] != 'S')
        startIndex++;
    int startRow = startIndex / (maze.width + 1);
    int startColumn = startIndex % (maze.width + 1);
    maze.distances[startRow * maze.width + startColumn] = 0;

    // initialise moving nodes (up to 4 nodes)
    Node_t movingNodes[4];
    for(int i = 0; i < 4; i++) 
    {
        movingNodes[i].column = startColumn;
        movingNodes[i].row = startRow;
        movingNodes[i].distance = 0;
    }

    // find how many nodes should exist at starting conditions
    int nMovingNodes = 0;
    for(int i = 0; i < 4; i++)
        nMovingNodes += moveNoteToNextAvailablePosition(&maze, &movingNodes[i]);

    // follow all the paths until a loop is found
    int nNodesMovedThisRound = nMovingNodes;
    while(nNodesMovedThisRound)
    {
        nNodesMovedThisRound = 0;
        for(int i = 0; i < nMovingNodes; i++)
            nNodesMovedThisRound += moveNoteToNextAvailablePosition(&maze, &movingNodes[i]);
    }

    //debug
    for(int r = 0; r < maze.heigth; r++)
    {
        for(int c = 0; c < maze.width; c++)
        {
            int d = maze.distances[r * maze.width + c];
            if(d == -1) printf(".");
            else printf("%d", d);
        }
        printf("\n");
    }

    //cleanup 
    free(maze.distances);
    free((char*)maze.map);

    printf("Result: %d\n", finalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}