#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"

typedef struct Node_t
{
    int row, column;
} Node_t;

Node_t startDirection;
char lastNodeChar = '\0';

typedef struct
{
    char* map;
    int width, heigth; 
} Maze_t;

/**!
 * Determines if a character is part of the 'modified loop'.
 * This is useful to identify intesections with the loop.
 * @param c the map char
 * @return `1` if there's part of the loop pipes. `0` otherwise.
*/
char isPartOfNewLoop(char c)
{
    return c >= '1' && c <= '6';
}

/**!
 * Converts the original character into a new character.
 * This function is used so we can mofify the maze and destinguish the loop parts
 * from the other parts outside the loop.
 * This is needed because all parts (other than '.') which are outside the loop should
 * count as `I`.
 * @param original the original character
 * @return the new character
*/
char originalToNewChar(char original)
{
    switch (original)
    {
        case '|': return '1';
        case '-': return '2';          
        case 'L': return '3';           
        case 'J': return '4';          
        case '7': return '5';
        case 'F': return '6';

        default: return original;
    }
}

/**!
 * Converts the new character into the original character.
 * This function is used to invert `originalToNewChar` for debugging purposes.
 * @param original the new character
 * @return the original character
*/
char newToOriginalChar(char new)
{
    switch (new)
    {
        case '1': return '|';
        case '2': return '-';          
        case '3': return 'L';           
        case '4': return 'J';          
        case '5': return '7';
        case '6': return 'F';
        default: return new;
    }
}

/**!
 * Determines if a connection is possible based on the current position and a moviment direction.
 * It computes this result based on the current node tile, the target tile and if the target was 
 * already visited.
 * @param maze a pointer to the current problem maze
 * @param currentNode a pointer to the current position node
 * @param deltaRow the vertical moviment direction. `-1`, `0` or `1`
 * @param deltaColumn the horizontal moviment direction. `-1`, `0` or `1`
 * @return `1` if there's a connection. `0` otherwise.
*/
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

    // check if destination has a connection to current node
    char destinationChar = maze->map[destinationRow * (maze->width + 1) + destinationColumn];
    if(destinationChar == '.') return 0;
    if(destinationChar == '|' && deltaColumn != 0) return 0;
    if(destinationChar == '-' && deltaRow != 0) return 0;
    if(destinationChar == 'L' && (deltaRow == -1 || deltaColumn == 1)) return 0;
    if(destinationChar == 'J' && (deltaRow == -1 || deltaColumn == -1)) return 0;
    if(destinationChar == '7' && (deltaRow == 1 || deltaColumn == -1)) return 0;
    if(destinationChar == 'F' && (deltaRow == 1 || deltaColumn == 1)) return 0;

    // check if destination is unexplored / is the initial node we just came from
    if(isPartOfNewLoop(destinationChar)) return 0;
    // hack: so we don't visit the original node twice
    if(destinationChar == 'S' && lastNodeChar == 'S') return 0;
    
    // all tests passed. Connection is possible
    return 1;
}

/**!
 * Moves a given note to the first available and possible adjacent position on maze.
 * @param maze a pointer to the current problem maze
 * @param currentNode a pointer to the current position node
 * @return `1` if the node moved. `0` otherwise.
*/
int moveNoteToNextAvailablePosition(Maze_t* maze, Node_t* currentNode)
{                       //left, right, up, down
    int deltaRows[] =    {  0,  0, -1,  1 };
    int deltaColumns[] = { -1,  1,  0,  0 };

    // check all 4 adjacent spots
    for(int i = 0; i < 4; i++) 
    {
        if(!hasConnectionTo(maze, currentNode, deltaRows[i], deltaColumns[i])) continue;

        // found a connection. move the current node and finish
        int currentMazeIndex = currentNode->row * (maze->width + 1) + currentNode->column;
        char currentChar = maze->map[currentMazeIndex];
        maze->map[currentMazeIndex] = originalToNewChar(currentChar);

        // store the last node char and move forward
        lastNodeChar = currentChar;
        currentNode->column += deltaColumns[i];
        currentNode->row += deltaRows[i];

        // used so we know the direction the maze started
        // this is important so we can replace 'S' with the correct symbol.
        // for example, if 'S' has a left and top connection it should re
        // replaced by a 'J'.
        // this is important so the algorithm can later identify the edges / borders
        // properly  
        if(currentChar == 'S') 
        {
            startDirection.row = deltaRows[i];
            startDirection.column = deltaColumns[i];
        }

        // compute and replace 'S' with the right piece of pipe
        // just as described on the upper commentary
        currentMazeIndex = currentNode->row * (maze->width + 1) + currentNode->column;
        char nextChar = maze->map[currentMazeIndex];
        if(nextChar == 'S')
        {
            switch (i)
            {
            case 0: // coming from right
                if(startDirection.column == -1)
                    nextChar = '-';
                else if(startDirection.row == 1)
                    nextChar = 'F';
                else if(startDirection.row == -1)
                    nextChar = 'L';
                break;
            case 1: // coming from left
                if(startDirection.column == 1)
                    nextChar = '-';
                else if(startDirection.row == 1)
                    nextChar = 'J';
                else if(startDirection.row == -1)
                    nextChar = '7';
                break;
            case 2: // coming from down
                if(startDirection.column == -1)
                    nextChar = '7';
                else if(startDirection.column == 1)
                    nextChar = 'F';
                else if(startDirection.row == -1)
                    nextChar = '|';
                break;
            case 3: // coming from top
                if(startDirection.column == -1)
                    nextChar = 'J';
                else if(startDirection.column == 1)
                    nextChar = 'L';
                else if(startDirection.row == 1)
                    nextChar = '|';
                break;
            default:
                break;
            }
            maze->map[currentMazeIndex] = originalToNewChar(nextChar);
        }
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

    // find S (start)
    int startIndex = 0;
    while(maze.map[startIndex] != 'S')
        startIndex++;
    int startRow = startIndex / (maze.width + 1);
    int startColumn = startIndex % (maze.width + 1);

    // initialise moving node
    Node_t movingNode;
    for(int i = 0; i < 4; i++) 
    {
        movingNode.column = startColumn;
        movingNode.row = startRow;
    }
    startDirection.column = 0;
    startDirection.row = 0;

    // run though the maze and modify the characters so we can identify the loop
    // this is done so the loop is preserved while the unimportant junk can count as 
    // outside the loop, even if it's a pipe part and not a '.'
    int didMove = 0;
    do 
    {
        didMove = moveNoteToNextAvailablePosition(&maze, &movingNode);
    }
    while(didMove);

    // compute result by using an adapted version of the winding number algorithm
    int totalResult = 0;
    for(int r = 0; r < maze.heigth; r++)
    {
        int isInside = 0;
        // keep tack of the last corner so we can decide if isInside should be toggled once 
        // the horizontal line ends
        char lastCorner = '\0';
        for(int c = 0; c < maze.width; c++)
        {
            char currentChar = maze.map[r * (maze.width + 1) + c];

            // it's a left corner. it's the start of an horizontal line
            if((currentChar == originalToNewChar('F') || currentChar == originalToNewChar('L')))
            {
                // keep the last corner in memory so we can 
                // take a decision once the horizontal line ends
                lastCorner = currentChar;
            }

            // check for corners with same directions (UP-left-UP / DOWN-right-DOWN)
            else if(currentChar == originalToNewChar('7'))
            {
                // only increment if the other corner matches
                if(lastCorner == originalToNewChar('L'))
                    isInside = ++isInside % 2;
            }

            // check for corners with same directions (UP-left-UP / DOWN-right-DOWN)
            else if(currentChar == originalToNewChar('J'))
            {
                // only increment if the other corner matches
                if(lastCorner == originalToNewChar('F'))
                    isInside = ++isInside % 2;
            }

            // crossed a border. toggle isInside
            else if(isPartOfNewLoop(currentChar) && currentChar != originalToNewChar('-'))
            {
                isInside = ++isInside % 2;
            }

            // found something that's not part of the loop
            // place 'I' if it's inside. '0' otherwise.
            else if(!isPartOfNewLoop(currentChar))
            {
                if(isInside)
                {
                    maze.map[r * (maze.width + 1) + c] = 'I';
                    totalResult++;
                }
                else {
                    maze.map[r * (maze.width + 1) + c] = '0';
                }
            }
        }
    }

    /*debug
    for(int r = 0; r < maze.heigth; r++)
    {
        for(int c = 0; c < maze.width; c++)
        {
            char d = maze.map[r * (maze.width + 1) + c];
            if(d == -1) printf(".");
            else printf("%c", newToOriginalChar(d));
        }
        printf("\n");
    }*/

    //cleanup 
    free((char*)maze.map);

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}