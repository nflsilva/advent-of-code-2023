#include "cutil/math.h"

float cutilMathClipf(float value, float max, float min)
{
    if(max < min) return value;
    if(value > max) return max;
    if(value < min) return min;
    return value;
}