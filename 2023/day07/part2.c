#include "common.h"
#include "list.h"

typedef struct Hand {
    int cards[5];
    int bid;
    enum {
        HighCard,
        OnePair,
        TwoPair,
        ThreeOfAKind,
        FullHouse,
        FourOfAKind,
        FiveOfAKind
    } type;
} Hand;

void print_hand(Hand * hand) {
    println("Cards: [{5i:, }], Bid: {i}, Type: {i}", hand->cards[0], hand->cards[1], hand->cards[2], hand->cards[3], hand->cards[4], hand->bid, hand->type);
}

int card_to_numeric(char card) {
    switch (card) {
        case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            return card - '0';
        case 'T':
            return 10;
        case 'J': // joker
            return 0;
        case 'Q': 
            return 12;
        case 'K':
            return 13;
        case 'A':
            return 14;
    }

    println("Invalid card characther: {c}", card);
    exit(1);
}

int get_type_of_hand(Hand * hand) {
    int cards[13] = {0}; // Store the first occurance of the card in hand
    int amount[5] = {0}; /* Store the amount of times a card is seen. 
                            The correspondant card to an amount is found 
                            in the cards array above */
    
    int index, diff = 0, max = 1, jokers = 0;

    for (int i = 0; i < 5; ++i) {
        index = hand->cards[i];
        // joker
        if (index == 0) {
            jokers++;
            continue;
        }
        index = cards[index - 2];
        if (index) {
            if (max < ++amount[index - 1]) {
                max++;
            }
        } else {
            amount[diff] = 1;
            cards[hand->cards[i] - 2] = ++diff;
        }
    }

    max += jokers;
     
    switch (diff - max) {
        case 4:
            return HighCard;
        case 2:
            return OnePair;
        case 1:
            return TwoPair;
        case 0:
            return ThreeOfAKind;
        case -1:
            return FullHouse;
        case -2:
            return FourOfAKind;
        case -6:
        case -4:
            return FiveOfAKind;
    }

    println("Uhoh unknown type of card? Diff: {i}, Max: {i}", diff, max);
    exit(1);
}

int compare_hands(const void * first, const void * second) {
    const Hand * hand1 = first, * hand2 = second;

    if (hand1->type != hand2->type)
        return hand1->type < hand2->type;

    for (int i = 0; i < 5; ++i) {
        if (hand1->cards[i] != hand2->cards[i])
            return hand1->cards[i] < hand2->cards[i];
    }

    return 0;
}

int main() {
    start_timer();
    FILE * fp = fopen("input.txt", "r");

    if (fp == NULL) {
        println("File not found");
        exit(1);
    }

    char * line = NULL;
    size_t length = 0, read, sum = 0;

    struct List * hands = init_list(sizeof(Hand *));
    Hand * hand;

    while ((read = getline(&line, &length, fp)) != -1) {
        hand = malloc(sizeof(Hand));
        for (int i = 0; i < 5; ++i) {
            hand->cards[i] = card_to_numeric(line[i]);
        }
        hand->bid = atoi(line + 5);
        hand->type = get_type_of_hand(hand);
        list_push(hands, hand);
    }

    list_sort(hands, &compare_hands); 

    for (int rank = 0; rank < hands->size; ++rank) {
        sum += ((Hand *) hands->items[rank])->bid * (rank + 1);
    }

    printf("Execution time: %.3fms\n", stop_timer());
    println("Result: {li}", sum);
}
