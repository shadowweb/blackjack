#include <stdlib.h>
#include <stdio.h>

#include "deck.h"

static char *swNameString[] =
{
    "Ace",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "Jack",
    "Queen",
    "King"
};

static char *swSuiteString[] =
{
    "Spades",
    "Hearts",
    "Diamonds",
    "Clubs"
};

static uint8_t swNameValue[] =
{
    11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10
};

swDeck *swDeckNew()
{
    swDeck *deck = calloc(1, sizeof(swDeck));
    if (deck)
    {
        for (uint8_t i = 0; i < DECK_SIZE; i++)
        {
            deck->cards[i].suite = i/MAX_NAME;
            deck->cards[i].name = i%MAX_NAME;
        }
    }
    return deck;
}

void swDeckDelete(swDeck *deck)
{
    if (deck)
        free(deck);
}

void swDeckShuffle(swDeck *deck)
{
    if (deck)
    {
        printf ("\n*** Deck Shuffle\n");
        for (uint8_t i = 0; i < DECK_SIZE; i++)
        {
            uint8_t position = rand()%DECK_SIZE;
            swCard tempCard = deck->cards[i];
            deck->cards[i] = deck->cards[position];
            deck->cards[position] = tempCard;
        }
        deck->next = 0;
    }
}

swCard *swDeckNextGet(swDeck *deck)
{
    swCard *card = NULL;
    if (deck && deck->next < DECK_SIZE)
    {
        card = &(deck->cards[deck->next]);
        deck->next++;
    }
    return card;
}

void swCardPrint(swCard *card)
{
    if (card)
        printf("{ %s, %s }", swNameString[card->name], swSuiteString[card->suite]);
}

uint8_t swCardValueGet(swCard *card)
{
    if (card)
        return swNameValue[card->name];
    return 0;
}

void swHandPrint(swHand *hand, bool all, char *str)
{
    if (hand)
    {
        if (str)
            printf("%s ", str);
        if (all)
        {
            printf ("Cards: ");
            for (uint8_t i = 0; i < hand->count; i++)
                swCardPrint(hand->cards[i]);
            printf ("; Value: %u\n", hand->value);
        }
        else
        {
            printf ("Cards: { X, X }");
            if (hand->count)
                swCardPrint(hand->cards[hand->count - 1]);
            printf ("\n");
        }
    }
}

