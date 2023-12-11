#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* allSequences[100];
int nSequences = 0;

/**!
 *  Recursive function used to append the new numbers at the end of the sequence
 * @param sequence the pointer to an array of sequence elements
 * @param length the length of the array of sequence
 * @return the number added last to the sequence
*/
int processSequence(int* sequence, int length)
{
    // iterate over the sequence and compute new sequence
    int sum = 0;
    int* newSequence = (int*) malloc(sizeof(int) * (length));
    memset(newSequence, 0, sizeof(int) * (length));
    allSequences[nSequences++] = newSequence;

    for(int i = 1; i < length; i++)
    {
        newSequence[i - 1] = sequence[i] - sequence[i - 1];
        sum += newSequence[i - 1];
    }
    
    // iterate over this new sequence if it's not all zeros
    int result = 0;
    if(sum != 0)
        result += processSequence(newSequence, length - 1);
    sequence[length] = sequence[length - 1] + newSequence[length - 1];

    // new sequence no longer needed
    return sequence[length];
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

    // buffer for first sequence
    int totalResult = 0;
    int sequenceNumbers[1024] = { 0 };
    
    // quick hack to read a specific number of lines from input file
    for(int l = 0; l < 200; l++)
    {
        int sequenceNumbersLength = 0;
        while(fscanf(file, "%d", &sequenceNumbers[sequenceNumbersLength]) == 1) 
        {
            sequenceNumbersLength++;
            if(fgetc(file) == '\n')
                break;
        }
        totalResult += processSequence(sequenceNumbers, sequenceNumbersLength);

        for(int i = 0; i < 0; i++)
            free(allSequences[i]);
        nSequences = 0;
    }

    // close file, as it's no longer needed
    fclose(file);

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}