#ifndef _CUTIL_MATH_H
#define _CUTIL_MATH_H

/*!
 * Returns input value clipped by two `float`.
 * @param value the original value to clip
 * @param max the minimum value
 * @param min the maximum value
 * @return The clipped value
 * @warning If `max < min` the original is returned
 */
float cutilMathClipf(float value, float max, float min);

/*!
 * Computes the greater common divisor of two numbers.
 * @param n0 the first number
 * @param n1 the second number
 * @return the GCD between all the numbers
 */
long long int cutilMathGCD(long long int n0, long long int n1);

/*!
 * Computes the least common divisor of two numbers.
 * @param numbers a pointer to an array of numbers
 * @param length the length of the array of numbers
 * @return the LSM between all the numbers
 */
long long int cutilMathLCM(long long int* numbers, int length);

#endif