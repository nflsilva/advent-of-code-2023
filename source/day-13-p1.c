#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"

/**
 * Determines if there's a column wise mirror by searching for a repeating column and then
 * verifying the mirror until the matrix ends
 * @param buffer the buffer with the matrix input
 * @param width the width of the matrix
 * @param height the height of the matrix
 * @return the number of columns (from the left) until the mirror happends. `0` if 
 * no mirroring is present
*/
int hasColumnMirroring(const char* buffer, int width, int height)
{
    int foundMirror = 0;
    int c = 0;

    while(c != width)
    {
		// find the repeating column
        for(; c < width - 1; c++)
        {
            foundMirror = 1;
            for(int r = 0; r < height; r++)
            {
                int index = r * (width + 1) + c;
                if(buffer[index] != buffer[index + 1]) 
                {
                    foundMirror = 0;
                    break;
                }
            }
            if(foundMirror)
                break;
        }
        
		// no repeating column found
        if(!foundMirror) return 0;
    
		// validate the columns around the repeating ones
        for(int lc = c - 1, rc = c + 2; lc >= 0 && rc < width; lc--, rc++)
        {
            for(int r = 0; r < height; r++)
            {
                int leftIndex = r * (width + 1) + lc;
                int rightIndex = r * (width + 1) + rc;
                char c0 = buffer[leftIndex];
                char c1 = buffer[rightIndex];
                if(buffer[leftIndex] != buffer[rightIndex]) 
                {
                    foundMirror = 0;
                    break;
                }   
            }
        }

		// mirroring was validated. return result
        if(foundMirror) return (c + 1);
        c += 2;

    }
    return 0;
}

/**
 * Determines if there's a row wise mirror by searching for a repeating row and then
 * verifying the mirror until the matrix ends
 * @param buffer the buffer with the matrix input
 * @param width the width of the matrix
 * @param height the height of the matrix
 * @return the number of rows (from the top) until the mirror happends
*/
int hasRowMirroring(const char* buffer, int width, int height)
{

    int foundMirror = 0;
    int r = 0;

    while(r != height - 1)
    {
		// search for the repeating rows
        for(; r < height - 1; r++)
        {
            foundMirror = 1;
            for(int c = 0; c < width; c++)
            {
                int index0 = r * (width + 1) + c;
                int index1 = (r + 1) * (width + 1) + c;
                if(buffer[index0] != buffer[index1]) 
                {
                    foundMirror = 0;
                    break;
                }
            }

            if(foundMirror)
                break;
        }

		// no repeating column found
        if(!foundMirror) return 0;

		// validate the columns around the repeating ones
        for(int tr = r, br = r + 1; tr >= 0 && br < height; tr--, br++)
        {
            for(int c = 0; c < width; c++)
            {
                int index0 = tr * (width + 1) + c;
                int index1 = br * (width + 1) + c;
                char c0 = buffer[index0];
                char c1 = buffer[index1];
                if(c0 != c1)
                {
                    foundMirror = 0;
                    break;
                }   
            }
        }

		// mirroring was validated. return result
        if(foundMirror) return 100 * (r + 1);
        r += 2;
    }
    return 0;
}

int readPatternFromBuffer(const char* bufferPointer, int* charsRead)
{
    int width = 0, height = 0;

    // find width
    while(bufferPointer[width] != '\n')
        width++;
    
    // find height
    while(bufferPointer[height * (width + 1)] > '\n')
        height++;

    // add chars to advance buffer pointer
    *charsRead = ((width + 1) * height) + 1;

    // validate mirroring
    int r0 = hasRowMirroring(bufferPointer, width, height);
	int r1 = hasColumnMirroring(bufferPointer, width, height);
    return r0 + r1;
}

int main(int argc, char** argv)
{

    if(argc < 2)
    {
        printf("No file provided");
        exit(EXIT_FAILURE);
    }
    char* filePath = argv[1];
    
    // read all file into memory
    const char* fileData = cutilFilesystemLoadFile(filePath);
    if(!fileData)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }
    int fileDataLength = strlen(fileData);

    int totalResult = 0;
    int totalCharsRead = 0;
    while (totalCharsRead < fileDataLength)
    {
        int charsRead;
        totalResult += readPatternFromBuffer(fileData + totalCharsRead, &charsRead);
        totalCharsRead += charsRead;
    }

    free((void*)fileData);
    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}