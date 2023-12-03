#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "cutil/filesystem.h"

int main()
{

    const char* input = cutilFilesystemLoadFile("resources/day-1-input.txt");
    if(!input)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    unsigned int totalResult = 0;
    unsigned int lineResult = 0;
    // + 1 added to take care for edge case in which input doesn't have '\n' on last line
    unsigned int inputSize = strlen(input) + 1;

    for(int i = 0; i < inputSize; i++)
    {
        char c = input[i];

        // char is a text character. ignore.
        if(c < '0' || c > '9') continue;

        // end of line / file
        // add the last number found
        if(c == '\n' || c == '\0') {
            totalResult += lineResult;
            lineResult = 0;
            continue;
        }

        // it's the first number on the text line. must the x10
        int intc = c - '0';
        if(lineResult == 0) 
            totalResult += intc * 10;

        // sets the last number found on the line
        lineResult = intc;

    }
    
    printf("\n\nResult: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}