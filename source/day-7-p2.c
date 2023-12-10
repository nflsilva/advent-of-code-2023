#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/sort.h"

#define N_CARDS_IN_HAND 5
#define N_DIFFERENT_CARDS 14
#define N_HANDS_MAX 1000
#define PAIR_STRENGTH 10000000000ll
#define OTHERS_STRENGTH 20000000000ll

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
        case 'J': return 0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return label - '0';
        case 'T': return 10;
        case 'Q': return 11;
        case 'K': return 12;
        case 'A': return 13;
        default: return -1;
    }
}

/**! 
 *  Calculates the Hand strength based on the last number of cards
 * 1 pair = 1 point 
 * 2 pairs => 1 pair x 2 = 2 points
 * 3 of a kind => 1 pair + another = 3 points
 * full house 1 pair and 3 of a kind => 1 + 3 = 4 points
 * 4 of a kind => 3 of a kind + 2 = 5 points
 * 5 of a kind => 4 of a kind + 1 = 7 points
 * 
 *  @param hand the hand to compute the new strength value
 *  @param cardValue the index for the card to use to compute the hand strength
 *  @warning this function is `commulative`. It needs to be called each time the number 
 * of cards is updated so the hard strenght is computed correctly. This is done so 
 * this computation takes less time.
*/
void updateHandStrength(Hand_t* hand, int cardValue)
{
    switch (hand->nOfEachCard[cardValue])
    {
        case 2: 
            hand->stength += PAIR_STRENGTH; 
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
            hand->stength += OTHERS_STRENGTH;      
        default:    
            break;
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
        int highestCardCountIndex = 0;
        int highestCardCount = 0;
        int nJokers = 0;

        // read cards and compute hand strength
        char card;
        int cardIndex = 0;
        long long cardValueMultiplier = 100000000ll;
        while((card = fgetc(file)) != ' ')
        {
            int cardValue = getCardValue(card);
            hand->stength += cardValue * cardValueMultiplier;

            // first cards are worth more on the comparison criteria
            cardValueMultiplier *= 0.01;
            if(cardValue == 0) 
            {
                // Joker cards will be used last
                nJokers++;
                continue;
            }
            // find the higher count card so Jokers can join them
            hand->nOfEachCard[cardValue]++;
            if(highestCardCount < hand->nOfEachCard[cardValue])
            {
                highestCardCount = hand->nOfEachCard[cardValue];
                highestCardCountIndex = cardValue;
            }

            // update strength since this card is not a Joker
            updateHandStrength(hand, cardValue);
        }

        // use Joker cards the best possible
        for(int i = 0; i < nJokers; i++)
        {
            hand->nOfEachCard[highestCardCountIndex]++;
            updateHandStrength(hand, highestCardCountIndex);
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