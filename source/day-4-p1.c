#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int scanNextNumber(char* line, int* offset)
{
    // scan single number
    int scannedNumber = 0;
    if(!sscanf(line + *offset, "%d ", &scannedNumber))
        return -1;
    *offset += 2;

    // add an adicional step if it's a 2 space number
    if(scannedNumber > 9) { *offset += 1; }

    // add an adicional step if the next number is a 1 space number
    if(line[*offset] == ' ') {*offset += 1; }

    return scannedNumber;
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

    // holds the numbers for the current scratchboard
    int winningNumbers[10];
    int winningNumbersIndex = 0;

    // holds the current line context
    char* lineBuffer = NULL;
    size_t lineLen = (size_t)NULL;
    unsigned int lenToProcess = 0;

    // for every line
    while ((lenToProcess = getline(&lineBuffer, &lineLen, file)) != -1)
    {
        // consume and discard the game id
        unsigned int lineOffset = 4;
        int cardId = 0;

        // hack to discard card number
        while(lineBuffer[lineOffset] != ':')
            lineOffset++;

        // discard ': '
        lineOffset += 2;
        
        // scan each winning number
        int scannedNumber = -1;
        while((scannedNumber = scanNextNumber(lineBuffer, &lineOffset)) != -1)
            winningNumbers[winningNumbersIndex++] = scannedNumber;

        // discard '| ' 
        lineOffset += 2;

        // scan each scratchboard number
        // frute-force check if it's a winning number
        int pointMultiplier = 1;
        int pointsThisCard = 0;
        while(lineOffset < lenToProcess)
        {
            scannedNumber = scanNextNumber(lineBuffer, &lineOffset);
            for(int i = 0; i < winningNumbersIndex; i++)
            {
                if(winningNumbers[i] == scannedNumber)
                {
                    printf("%d+%d ", scannedNumber, pointMultiplier);
                    pointsThisCard = pointMultiplier;
                    pointMultiplier *= 2;
                }
            }
        }

        // end of line
        // update result and reset counters
        totalResult += pointsThisCard;
        winningNumbersIndex = 0;
        if(pointsThisCard != 0)
            printf("\n");
    }

    fclose(file);
    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}