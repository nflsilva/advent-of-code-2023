#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_RACES 1

/** Explanation
 * analysing the problem we can conclude that winning options
 * will verify the following rule `n * (L - n) > R` where
 * `n` is the number of milliseconds to hold the button at the start
 * `L` is the length of the race
 * `R` is the record to beat
 * So, in this example
 * Time:      7  15   30
 * Distance:  9  40  200
 * holding the button for 2 milliseconds will give us 
 * `2 * (7 - 2) > 9` => `2 * 5 > 9` => `10 > 9` thus meaning it's a win
 * exploring this idea further, we can see that the presented formula is quadratic
 * and is equivalente to -n² + Ln - R > 0
 * so, by finding the roots of this parabole, we can find which numbers are the winning
 * ones.
 */

/**!
 * Holds the race info
 * @param time the max time of the race
 * @param distance the max distance of the race
*/
typedef struct {
    long long int time, distance;
} Race_t;

/**!
 * Solves a quadratic equation for the given coefficient
 * @param a the a coefficient
 * @param b the b coefficient
 * @param c the c coefficient
 * @param r0 the first root result
 * @param r1 the second root result
*/
void solveQuadraticEquation(
    long long int a, 
    long long int b, 
    long long int c, 
    double* r0, 
    double* r1)
{
    double numerator = sqrt(b*b - 4 * a * c);
    double denominator = 2 * a;
    *r0 = floor(((-b + numerator) / denominator) + 1);
    *r1 = ceil(((-b - numerator) / denominator) - 1);
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("No file provided");
        exit(EXIT_FAILURE);
    }
    char* filePath = argv[1];
    
    int totalResult = 1;
    Race_t race;

    FILE* file = fopen(filePath, "r");
    if(!file)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    // discard 'Time:'
    fscanf(file, "Time:");
    // read distances
    fscanf(file, "%lld\n", &race.distance);
    // discard 'Distance:'
    fscanf(file, "Distance:");
    // read records 
    fscanf(file, "%lld\n", &race.time);
    // close the file. not needed anymore
    fclose(file);

    // compute the range of winning plays by solving the quadratic equation
    double r0, r1;
    solveQuadraticEquation(-1, race.distance, -race.time, &r0, &r1);
    totalResult *= (int)(r1 - r0) + 1;

    printf("Result: %d\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}