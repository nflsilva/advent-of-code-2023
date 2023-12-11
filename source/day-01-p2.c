#include <stdio.h>
#include <string.h>

#include "cutil/filesystem.h"

#define MAX_NUMBERS 9
#define MAX_NUMBER_NAME 6

typedef struct spelled_number_t {
    const char name[MAX_NUMBER_NAME];
    const int nameLen;
    int searchIndex;
} spelled_number_t;

spelled_number_t numbers[MAX_NUMBERS] = {
    {"one", 3, 0},
    {"two", 3, 0},
    {"three", 5, 0},
    {"four", 4, 0},
    {"five", 4, 0},
    {"six", 3, 0},
    {"seven", 5, 0},
    {"eight", 5, 0},
    {"nine", 4, 0}
};

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

    // + 1 added to take care for edge case in which input doesn't have '\n' on last line
    unsigned int inputSize = strlen(input) + 1;
    unsigned int totalResult = 0;
    unsigned int lineResult = 0;
    char lastChar = '\0';
    
    unsigned int intcb = 0;
    char lineBuff[1024];
    unsigned int lineBuffIndex = 0;

    for(int i = 0; i < inputSize; i++)
    {
        char c = input[i];
        lineBuff[lineBuffIndex++] = c;

        // edge case when chars are repeated
        if(lastChar == c)
        {
            for(int n = 0; n < MAX_NUMBERS; n++)
            {
                if(numbers[n].name[numbers[n].searchIndex] != c)
                    numbers[n].searchIndex = 0;
            }
        }
        lastChar = c;

        // end of line / file
        // add the last number found
        // reset all search indexes
        if(c == '\n' || c == '\0') {

            lineBuff[lineBuffIndex - 1] =  '\0';
            lineBuffIndex = 0;
            intcb = 0;

            totalResult += lineResult;
            lineResult = 0;
            for(int n = 0; n < MAX_NUMBERS; n++)
                numbers[n].searchIndex = 0;

            continue;
        }

        // find out if the next number if a number or is spelled in text.
        unsigned int intc = -1;
        if(c < '0' || c > '9')
        {
            // next char is a text character
            for(int n = 0; n < MAX_NUMBERS; n++)
            {
                int searchIndex = numbers[n].searchIndex;
                if(numbers[n].name[searchIndex] == c)
                {
                    // the char matches the name of the number
                    numbers[n].searchIndex++;
                }  
                else 
                {
                    // the char doesn't match. Reset search for this number
                    numbers[n].searchIndex = 0;
                    continue;
                }   

                // last substring matches the name of number `n`. 
                // set `n + 1` as the next number on this text line 
                if(numbers[n].searchIndex == numbers[n].nameLen)
                {
                    intc = n + 1;
                    numbers[n].searchIndex = 0;
                }
            }
        }
        else 
        {
            // next char is a number.
            // set its value to the next number on this line and reset all searches
            intc = c - '0';
            for(int n = 0; n < MAX_NUMBERS; n++)
                numbers[n].searchIndex = 0;
        }

        // no number was found. moving one
        if(intc == -1) continue;

        // it's the first number on the text line. must the x10
        if(lineResult == 0) {
            intcb = intc;
            totalResult += intc * 10;
        }


        // sets the last number found on the line
        lineResult = intc;

    }

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}