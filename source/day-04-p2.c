#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_WINNER_NUMBERS 10
#define TOTAL_CARDS 204

/**!
 * Scans line for the next number, incrementing the offset by the
 * appropriated amount
 * @param line the line to scan
 * @param offset a pointer to the offset number to use when scanning the line
 * @warning this function will increment 
*/
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
    int winningNumbers[TOTAL_WINNER_NUMBERS];
    int winningNumbersIndex = 0;
    int numberOfCards[TOTAL_CARDS] = { 0 };

    // holds the current line context
    char* lineBuffer = NULL;
    size_t lineLen = (size_t)NULL;
    unsigned int lenToProcess = 0;

    // for every line
    int currentCardIndex = 0;
    while ((lenToProcess = getline(&lineBuffer, &lineLen, file)) != -1)
    {
        numberOfCards[currentCardIndex]++;

        // consume and discard the game number
        unsigned int lineOffset = 4;

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
        int wonCardIndex = currentCardIndex + 1;
        while(lineOffset < lenToProcess)
        {
            // scan the next number
            // search of scanned number is part of winning numbers
            scannedNumber = scanNextNumber(lineBuffer, &lineOffset);
            for(int i = 0; i < winningNumbersIndex; i++)
            {
                if(winningNumbers[i] == scannedNumber)
                {
                    // updates this card count
                    numberOfCards[wonCardIndex] += numberOfCards[currentCardIndex];
                    wonCardIndex++;
                }
            }
        }

        // end of line
        // update result and reset counters
        winningNumbersIndex = 0;
        currentCardIndex++;
    }

    // compute final result by adding all the cards
    for(int i = 0; i < TOTAL_CARDS; i++)
        totalResult += numberOfCards[i];

    fclose(file);
    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}