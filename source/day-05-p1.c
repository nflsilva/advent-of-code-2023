#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_COLUMNS 3
#define N_MAPS 7

typedef struct {
    unsigned long* table;
    unsigned int size;
}Map_t;

/**! Reads a map from file
 * @param file the file to read from
 * @param map the output map
 * @return the number of map rows
*/
unsigned int readMap(FILE* file, unsigned long** map)
{
    // discard chars until first number
    fscanf(file, "%*s %*s\n");

    // alloc 200 lines
    *map = (unsigned long*) malloc(sizeof(unsigned long) * 200 * MAP_COLUMNS);
    unsigned int nLines = 0;

    // read values until no lines left
    while(fscanf(file, "%d %d %d\n", *map + (nLines * MAP_COLUMNS + 0), *map + (nLines * MAP_COLUMNS + 1), *map + nLines * MAP_COLUMNS + 2) > 0)
        nLines++;

    return nLines;
}

/**! Computes the correct destination based on a source and the source-to-destination map
 * @param source the source
 * @param map the source-to-destination map
 * @param mapLength the number of rows on source-to-destination map
 * @return the computed destination
*/
unsigned long computeDestinationFromSource(unsigned long source, unsigned long* map, unsigned int mapLength)
{
    // destination should be the source if no mapping was found
    unsigned long destination = source;
    for(int i = 0; i < mapLength; i++)
    {
        // compute limites for the source, based on the map
        unsigned long sourceStart = map[i * MAP_COLUMNS + 1];
        unsigned long sourceLength = map[i * MAP_COLUMNS + 2];
        unsigned long sourceEnd = sourceStart + sourceLength - 1;

        // check if source fits the limits for this map's row
        if(source >= sourceStart && source <= sourceEnd)
        {
            // if so, compute a destination from this map's row and finish
            unsigned long destinationOffset = source - sourceStart;
            unsigned long destinationStart = map[i * MAP_COLUMNS + 0];
            destination = destinationStart + destinationOffset;
            break;
        }
    }
    return destination;
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

    unsigned long totalResult = -1;

    // consume and discard 'seeds:'
    fscanf(file, "seeds:");

    // read seeds and store in array
    unsigned int nSeeds = 0;
    unsigned long seeds[1024] = { 0 };
    while(fscanf(file, " %d", &seeds[nSeeds]) == 1)
        nSeeds++;

    // read maps into memory for easy access
    Map_t maps[N_MAPS];
    for(int i = 0; i < N_MAPS; i++)
        maps[i].size = readMap(file, &maps[i].table);

    // close file, as we don't need it anymore
    fclose(file);

    for(int i = 0; i < nSeeds; i++)
    {
        // for each seed
        // cicle through the maps computing the destination for each source
        // feeding the previous destination as source to the next computation
        unsigned long source = seeds[i];
        for(int i = 0; i < N_MAPS; i++)
            source = computeDestinationFromSource(source, maps[i].table, maps[i].size);

        // the last destination is the location we are looking for
        // (and would be the source for the next map, if there was any more)
        // check if this is the lowest location
        if(totalResult == -1 || totalResult > source)
            totalResult = source;
    }

    // cleanup
    for(int i = 0; i < 7; i++)
        free(maps[i].table);

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}