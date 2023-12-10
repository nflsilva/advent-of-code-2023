#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cutil/sort.h"

#define N_CARDS_IN_HAND 5
#define N_DIFFERENT_CARDS 14
#define N_HANDS_MAX 1000

typedef struct {
    short cards[N_CARDS_IN_HAND];
    short nOfEachCard[N_DIFFERENT_CARDS];
    long long stength;
    int bid;
} Hand_t;

int handComparator(const void* a, const void* b) {
    long long diff = ((Hand_t*)a)->stength - ((Hand_t*)b)->stength;
    if(diff > 0) return 1;
    else return 0;
}

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
        int cardIndex = 0;
        long long cardValueMultiplier = 100000000;
        while((card = fgetc(file)) != ' ')
        {
            int cardValue = getCardValue(card);
            hand->stength += cardValue * cardValueMultiplier;
            cardValueMultiplier *= 0.01;
            hand->cards[cardIndex++] = cardValue;

            switch (hand->nOfEachCard[cardValue])
            {
                case 1: 
                    // 1 pair = 100 points     
                    // 2 pairs => 1 pair x 2 = 2 points
                    hand->stength += 10000000000ll; 
                    break;    
                case 2:
                    // 3 of a kind => 1 pair + another = 3 points
                    // full house 1 pair and 3 of a kind => 1 + 3 = 4 points
                    hand->stength += 20000000000ll;
                    break; 
                case 3:   
                    // 4 of a kind => 3 of a kind + 2 = 5 points  
                    hand->stength += 20000000000ll;
                    break;
                case 4:     
                    // 5 of a kind => 4 of a kind + 1 = 7 points
                    hand->stength += 20000000000ll;      
                default:    
                    break;
            }
            hand->nOfEachCard[cardValue]++;
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
    for(int rank = 1; rank <= nHands; rank++)
        totalResult += hands[rank - 1].bid  * rank;

    printf("Result: %lld\n", totalResult);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}