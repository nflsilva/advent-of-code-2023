#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/sort.h"

#define N_CARDS_IN_HAND 5
#define N_DIFFERENT_CARDS 14
#define N_HANDS_MAX 1000

/**!
 * Represents a hand of cards.
 * @param nOfEachCard the number of cards of each type
 * @param strenght the strenght value for this hand
 * @param bid the bid value for this hand 
 * 
*/
typedef struct {
    short nOfEachCard[N_DIFFERENT_CARDS];
    long long stength;
    int bid;
} Hand_t;

/**!
 * Compares two hands based on strength. Used for sorting.
 * @param a hand one
 * @param b hand two
 * @return `1` if hand `a` > hand `b`. `0` otherwise.
 * 
*/
int handComparator(const void* a, const void* b) {
    long long diff = ((Hand_t*)a)->stength - ((Hand_t*)b)->stength;
    if(diff > 0) return 1;
    else return 0;
}

/**! 
 *  Transforms a Card char into a value / index.
 *  @param label the Card in char form
 *  @returns the Card value / index
*/
int getCardValue(char label)
{
    switch (label)
    {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return label - '1';
        case 'T': return 9;
        case 'J': return 10;
        case 'Q': return 11;
        case 'K': return 12;
        case 'A': return 13;
        default: return -1;
    }
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("No file provided");
        exit(EXIT_FAILURE);
    }
    char* filePath = argv[1];
    
    long long totalResult = 0;
    int nHands = 0;
    Hand_t hands[N_HANDS_MAX] = { 0 };

    FILE* file = fopen(filePath, "r");
    if(!file)
    {
        printf("Unable to read file\n");
        exit(EXIT_FAILURE);
    }

    // read hands and find how many cards of each type each hand has
    do
    {
        Hand_t* hand = &hands[nHands];

        // read cards and compute hand strength
        char card;
        long long cardValueMultiplier = 100000000;
        while((card = fgetc(file)) != ' ')
        {
            int cardValue = getCardValue(card);
            hand->stength += cardValue * cardValueMultiplier;
            cardValueMultiplier *= 0.01;
            hand->nOfEachCard[cardValue]++;
            switch (hand->nOfEachCard[cardValue])
            {
                case 2: 
                    hand->stength += 10000000000ll; 
                    break;    
                case 3:
                case 4:   
                case 5:
                    // 1 pair = 1 point    
                    // 2 pairs => 1 pair x 2 = 2 points
                    // 3 of a kind => 1 pair + another = 3 points
                    // full house 1 pair and 3 of a kind => 1 + 3 = 4 points
                    // 4 of a kind => 3 of a kind + 2 = 5 points       
                    // 5 of a kind => 4 of a kind + 1 = 7 points
                    hand->stength += 20000000000ll;      
                default:    
                    break;
            }
        }

        // read bid
        int bid;
        fscanf(file, "%d", &bid);
        hand->bid = bid;
        nHands++;
    } 
    while (fgetc(file) != EOF);
    // close file as it's no longer needed
    fclose(file);

    // sort hands by strenght
    cutilSortMergeSort(hands, sizeof(Hand_t), nHands, &handComparator);
    // compute final result
    for(int rank = 1; rank <= nHands; rank++)
        totalResult += hands[rank - 1].bid  * rank;

    printf("Result: %lld\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}