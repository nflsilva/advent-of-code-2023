#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

/**!
 * Returns the max value of cubes for each color for all games
 * @param character the first character of the color
 * @return the max value for each color
*/
int getColorMax(const char character)
{
    switch (character)
    {
        case 'r': return MAX_RED;
        case 'g': return MAX_GREEN;
        case 'b': return MAX_BLUE;
        default: return -1;
    }
}

int main()
{

    FILE* file = fopen("resources/day-2-input.txt", "r");
    if(!file)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    unsigned int totalResult = 0;
    unsigned int gameId = 0;

    char* lineBuffer;
    size_t lineLen;
    unsigned int lenToProcess;
    while ((lenToProcess = getline(&lineBuffer, &lineLen, file)) != -1) 
    {
        // find game id
        unsigned int lenProcessed = 7;
        sscanf(lineBuffer, "Game %d:", &gameId);
        if(gameId > 9) { lenProcessed++; }
        if(gameId > 99) { lenProcessed++; }
        
        // add result by default
        totalResult += gameId;

        // iterate over every game color
        int colorAmount = 0;
        char colorName[10];
        do
        {
            // try read amount and color pair
            if(!sscanf(lineBuffer + lenProcessed, " %d %s", &colorAmount, colorName))
            {
                printf("ERROR [%d %s] %s",colorAmount, colorName, lineBuffer);
                break;
            }

            // check if game is possible
            int maxForColor = getColorMax(colorName[0]);
            if(maxForColor < 0) 
            {   
                printf("Invalid color '%s'\n", colorName);
                break;
            } 

            // game is possible
            // remove id if impossible
            // next game, next game
            if(colorAmount > maxForColor)
            {
                totalResult -= gameId;
                break;
            }

            // keep track of the processed len
            lenProcessed += strlen(colorName) + 3;
            if(colorAmount > 9) lenProcessed++;
        } 
        while(lenProcessed < lenToProcess);
    }
    fclose(file);

    printf("\n\nResult: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}