#include "cutil/math.h"

float cutilMathClipf(float value, float max, float min)
{
    if(max < min) return value;
    if(value > max) return max;
    if(value < min) return min;
    return value;
}


long long int cutilMathGCD(long long int n0, long long int n1)
{
    while(n1 != 0)
    {
        long long int aux = n1;
        n1 = n0 % n1;
        n0 = aux;
    }
    return n0;
}

long long int cutilMathLCMLCM(long long int n0, long long int n1)
{
    if(n0 == 0 && n1 == 0) return 0;
    return (n0 * n1) / cutilMathGCD(n0, n1);
}

long long int cutilMathLCM(long long int* numbers, int length)
{
    long long int result = numbers[0];
    for (int i = 1; i < length; i++) {
        result = cutilMathLCMLCM(result, numbers[i]);
    }
    return result;
}