#ifndef _DECK_H_
#define _DECK_H_

#include <stdint.h>
#include <stdbool.h>

#define DECK_SIZE   52
#define SUITE_SIZE  13
#define HAND_SIZE   10

typedef enum {
    SPADES = 0,
    HEARTS,
    DIAMONDS,
    CLUBS
} swSuite;

typedef enum {
    ACE = 0,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    MAX_NAME
} swName;

typedef struct swCard
{
    swName name;
    swSuite suite;
} swCard;

typedef struct swDeck
{
    swCard cards[DECK_SIZE];
    uint32_t next;
} swDeck;

typedef struct swHand
{
    swCard *cards[HAND_SIZE];
    uint8_t value;
    uint8_t count;
    unsigned hasAce : 1;
} swHand;

swDeck *swDeckNew();
void swDeckDelete(swDeck *deck);
void swDeckShuffle(swDeck *deck);
swCard *swDeckNextGet(swDeck *deck);

void swCardPrint(swCard *card);
uint8_t swCardValueGet(swCard *card);

void swHandPrint(swHand *hand, bool all);

#endif