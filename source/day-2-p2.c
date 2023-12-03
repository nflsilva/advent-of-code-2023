#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**!
 * Returns the color index value for each color for all games
 * @param character the first character of the color
 * @return the index value for the color
*/
int getColorIndex(const char character)
{
    switch (character)
    {
        case 'r': return 0;
        case 'g': return 1;
        case 'b': return 2;
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

    unsigned int resultSum = 0;
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
        
        // iterate over every game color
        int colorAmount = 0;
        char colorName[10];
        int nNeededPerColor[3] = { 0, 0, 0};
        do
        {
            // try read amount and color pair
            if(!sscanf(lineBuffer + lenProcessed, " %d %s", &colorAmount, colorName))
            {
                printf("ERROR [%d %s] %s",colorAmount, colorName, lineBuffer);
                break;
            }

            // check if this is the max (minimum required) value for this color
            int colorIndex = getColorIndex(colorName[0]);
            if(colorIndex < 0) 
            {   
                printf("Invalid color '%s'\n", colorName);
                break;
            } 

            if(nNeededPerColor[colorIndex] < colorAmount)
                nNeededPerColor[colorIndex] = colorAmount;

            // keep track of the processed len
            lenProcessed += strlen(colorName) + 3;
            if(colorAmount > 9) lenProcessed++;
        } 
        while(lenProcessed < lenToProcess);

        int power = 1;
        for(int i = 0; i < 3; i++)
            power *= nNeededPerColor[i];
        resultSum += power;

        printf("[%d %d %d = %d] %s", nNeededPerColor[0], nNeededPerColor[1], nNeededPerColor[2], power, lineBuffer);

    }


    fclose(file);

    printf("\n\nResult: %d\n", resultSum);

    exit(EXIT_SUCCESS);
}