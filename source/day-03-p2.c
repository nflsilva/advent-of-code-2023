#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"

/**!
 * Represents a gear and keeps track of which numbers are associated to it
 * @param number0 the first number associated with this gear
 * @param number1 the second number associated with this gear
 * @warning this program assumes one gear should never
 * more than 2 numbers and a number should never have more than 1 gear
 * because it was never explained what should happend for this cases
*/
typedef struct Gear {
    int number0;
    int number1;
} Gear;

/**!
 * An array of gears to keep track of all gears from the input
*/
Gear* gears;

Gear* searchForAsterix(const char* input, int currentInputIndex, int width, int height)
{
    int currentRow = currentInputIndex / width;
    int currentColumn = currentInputIndex % width;

    int minRow = currentRow - 1; if(minRow < 0) { minRow = 0; };
    int maxRow = currentRow + 1; if(currentRow >= (height - 1)) { maxRow = currentRow; }

    int minColumn = currentColumn - 1; if(minColumn < 0) { minColumn = 0; };
    int maxColumn = currentColumn + 1; if(currentColumn >= (width - 2)) { maxColumn = currentColumn; }

    for(int r = minRow; r < maxRow + 1; r++)
        for(int c = minColumn; c < maxColumn + 1; c++)
        {
            int index = r * width + c;
            if(input[index] == '*')
            return &gears[index];
        }
    return NULL;
}

int main(int argc, char** argv)
{

    if(argc < 2)
    {
        printf("No file provided");
        exit(EXIT_FAILURE);
    }
    char* filePath = argv[1];
    
    const char* input = cutilFilesystemLoadFile(filePath);
    if(!input)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    // lets find out how long if the input
    int totalSize = strlen(input);
    int totalResult = 0;

    // keeps track of the current number being read
    int currentNumberBuffer[10];
    int currentNumberBufferIndex = 0;

    // keeps track of the reading process
    char currentInputChar = '\0';
    int currentInputIndex = 0;
    Gear* touchedGear = NULL;

    // find the length of the line
    int width = 0;
    do { currentInputChar = input[width++]; }  while(currentInputChar != '\n');
    int height = totalSize / width;

    // lets allocate memory for all the grid so we 
    // have fast access to these structs 
    gears = (Gear*)malloc(sizeof(Gear) * totalSize);

    // start iteration
    do 
    {
        currentInputChar = input[currentInputIndex];
        
        // char is a number
        if(currentInputChar >= '0' && currentInputChar <= '9') \
        {
            // check for symbol only if none was found up until now
            if(touchedGear == NULL)
                touchedGear = searchForAsterix(input, currentInputIndex, width, height);
            
            currentNumberBuffer[currentNumberBufferIndex++] = currentInputChar - '0';
        }

        // char is dot or end of line
        else 
        {
            // was parsing a number and had a symbol at some point
            if(currentNumberBufferIndex > 0 && touchedGear != NULL)
            {
                // convert buffer to an actual number
                int number = 0;
                int multiplier = 1;
                for(int i = currentNumberBufferIndex-1; i >= 0; i--) 
                {
                    number += currentNumberBuffer[i] * multiplier;
                    multiplier *= 10;
                }

                // associate this number to this gear
                if(touchedGear->number0 == 0)
                    touchedGear->number0 = number;
                else if(touchedGear->number1 == 0)
                    touchedGear->number1 = number;

                if(touchedGear->number0 != 0 && touchedGear->number1 != 0)
                    totalResult += touchedGear->number0 * touchedGear->number1;
            }

            // invalidate last number and symbol
            touchedGear = NULL;
            currentNumberBufferIndex = 0;
        }
        currentInputIndex++;

    } 
    while(currentInputChar != '\0');

    // clean up
    free(gears);

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}