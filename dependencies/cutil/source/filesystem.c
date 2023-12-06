#include "cutil/filesystem.h"

const char* cutilFilesystemLoadFile(const char* filePath)
{

    FILE *file = NULL;
    file = fopen(filePath, "r");
    if(!file) return NULL;

    unsigned long readSize = 0;
    unsigned long contentSize = 0;
    unsigned long fileContentBufferSize = CUTIL_FS_CHUNK_SIZE * 10;
    char* fileContentBuffer = (char*)malloc(sizeof(char) * (fileContentBufferSize + 1));
    int didError = 0;
    do 
    {
        readSize = fread(&fileContentBuffer[contentSize], sizeof(char), CUTIL_FS_CHUNK_SIZE, file);
        if (ferror(file)) 
        {
            didError = 1;
            break;
        }

        contentSize += readSize;
        if(contentSize == fileContentBufferSize) 
        {
            fileContentBufferSize = fileContentBufferSize + CUTIL_FS_CHUNK_SIZE * 10;
            char* newContent = realloc(fileContentBuffer, fileContentBufferSize);
            if(!newContent)
            {
                didError = 1;
                break;
            }
            fileContentBuffer = newContent;
        }
            
    } while (readSize == CUTIL_FS_CHUNK_SIZE);

    fclose(file);
    if(didError)
    {
        free(fileContentBuffer);
        return NULL;
    }

    fileContentBuffer[contentSize] = '\0';    
    return fileContentBuffer;
}