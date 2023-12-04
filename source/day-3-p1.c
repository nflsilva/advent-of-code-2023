#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"

int isSymbol(char tc)
{
    return !(
        (tc >= '0' && tc <= '9') ||
        (tc >= 'A' && tc <= 'Z') ||
        (tc >= 'a' && tc <= 'z') ||
        (tc == '.')
    ); 
}

char getFirstAdjancentSymbol(const char* input, int currentInputIndex, int width, int height)
{
    int currentRow = currentInputIndex / width;
    int currentColumn = currentInputIndex % width;

    int minRow = currentRow - 1; if(minRow < 0) { minRow = 0; };
    int maxRow = currentRow + 1; if(currentRow >= (height - 1)) { maxRow = currentRow; }

    int minColumn = currentColumn - 1; if(minColumn < 0) { minColumn = 0; };
    int maxColumn = currentColumn + 1; if(currentColumn >= (width - 2)) { maxColumn = currentColumn; }

    for(int r = minRow; r < maxRow + 1; r++)
        for(int c = minColumn; c < maxColumn + 1; c++)
            if(isSymbol(input[r * width + c]))
                return input[r * width + c]; 
    return '\0';
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

    int totalSize = strlen(input);
    int totalResult = 0;

    // keeps track of the current number being read
    int currentNumberBuffer[10];
    int currentNumberBufferIndex = 0;

    // keeps track of the reading process
    char currentInputChar = '\0';
    int currentInputIndex = 0;
    char currentSymbol = '\0';

    // find the length of the line
    int width = 0;
    do { currentInputChar = input[width++]; }  while(currentInputChar != '\n');
    int height = totalSize / width;

    // start iteration
    do 
    {
        currentInputChar = input[currentInputIndex];
        
        // char is a number
        if(currentInputChar >= '0' && currentInputChar <= '9') \
        {
            // check for symbol only if none was found up until now
            if(currentSymbol == '\0')
                currentSymbol = getFirstAdjancentSymbol(input, currentInputIndex, width, height);
            
            currentNumberBuffer[currentNumberBufferIndex++] = currentInputChar - '0';
        }

        // char is dot or end of line
        else 
        {
            // was parsing a number and had a symbol at some point
            if(currentNumberBufferIndex > 0 && currentSymbol)
            {
                // convert buffer to an actual number
                int multiplier = 1;
                for(int i = currentNumberBufferIndex-1; i >= 0; i--) 
                {
                    totalResult += currentNumberBuffer[i] * multiplier;
                    multiplier *= 10;
                }
            }

            // invalidate last number and symbol
            currentSymbol = '\0';
            currentNumberBufferIndex = 0;
        }

        currentInputIndex++;

    } while(currentInputChar != '\0');

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}