#include <assert.h>
#include <stdlib.h>

#include "cutil/math.h"

int main()
{
    // float tests
    assert(cutilMathClipf(2.0, -1.0, 1.0) == 2.0);
    assert(cutilMathClipf(2.0, 1.0, 0.0) == 1.0);
    assert(cutilMathClipf(1.0, 2.0, 1.5) == 1.5);

    exit(EXIT_SUCCESS);
}