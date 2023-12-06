#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/filesystem.h"

int main()
{

    static const char* expectedText = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    const char* dummyCode = cutilFilesystemLoadFile("dependencies/cutil/tests/assets/text.txt");

    assert(dummyCode != NULL);
    assert(strlen(dummyCode) == strlen(expectedText));
    assert(strcmp(dummyCode, expectedText) == 0);

    free((char*)dummyCode);

    exit(EXIT_SUCCESS);
}