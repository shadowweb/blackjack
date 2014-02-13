#include <stdlib.h>
#include <stdio.h>

#include "deck.h"

uint32_t gamesCount = 0;
uint32_t playerWinCount = 0;
uint32_t playerDrawCount = 0;
double playerPercentageWin = 0.0;

void gameEnd(bool playerWin, bool draw, swHand *playerHand, swHand *dealerHand)
{
    gamesCount++;
    if (playerWin)
    {
        playerWinCount++;
        playerPercentageWin = (double)playerWinCount/(double)gamesCount;
    }
    if (draw)
        playerDrawCount++;
    printf ("Game End: %s; Player Percentage Win: %f\n", ((playerWin)? "Player Won" : ((draw)? "Draw" : "Dealer Won" ) ), playerPercentageWin);
    swHandPrint(playerHand, true, "Player Hand:");
    swHandPrint(dealerHand, true, "Dealer Hand:");
}

bool deal(swDeck *deck, swHand *hand)
{
    bool rtn = false;
    if (deck && hand && hand->count < HAND_SIZE)
    {
        swCard *nextCard = swDeckNextGet(deck);
        if (nextCard)
        {
            hand->cards[hand->count] = nextCard;
            hand->value += swCardValueGet(nextCard);
            hand->count++;
            if (nextCard->name == ACE)
                hand->hasAce = true;
            rtn = true;
        }
    }
    return rtn;
}

enum
{
    NONE,
    HIT,
    STAND,
    QUIT
} action;

action getNextAction()
{
    action rtn = NONE;
    int actionChar = getchar();
    if (actionChar == 'h')
        rtn = HIT;
    else if (actionChar == 's')
        rtn = STAND;
    else
        rtn = QUIT;
    return rtn;
}

int main(int argc, char *argv)
{
    srand(time(NULL));
    swDeck *deck = swDeckNew();

    if (deck)
    {
        while (true)
        {
            bool done = false;
            swDeckShuffle(deck);
            for (uint8_t i = 0; i < 6; i++)
            {
                // play game
                swHand player = {0};
                swHand dealer = {0};
                bool failure = false;
                if (deal(deck, &player) && deal(deck, &dealer) && deal(deck, &player) && deal(deck, &dealer))
                {
                    swHandPrint(&player, true, "Player Hand:");
                    swHandPrint(&dealer, false, "Dealer Hand:");

                    if (player.value == 21)
                    {
                        if (dealer.value == 21)
                            gameEnd(false, true, &player, &dealer);
                        else
                            gameEnd(true, false, &player, &dealer);
                    }
                    else
                    {
                        action playerAction = NONE;
                        while ((playerAction = getNextAction()) == HIT)
                        {
                            printf("Player choose HIT.\n");
                            if (deal(deck, &player))
                            {
                                swHandPrint(&player, true, "Player Hand:");
                                if (player.value > 21)
                                    break;
                            }
                            else
                            {
                                failure = true;
                                break;
                            }
                        }
                        if (player.value > 21)
                            gameEnd(false, false, &player, &dealer);
                        else if (playerAction == STAND)
                        {
                            printf("Player choose STAND.\n");
                            while (true)
                            {
                                if ((dealer.value < 17) || ((dealer.value == 17) && dealer.hasAce))
                                {
                                    printf("Dealer choose HIT.\n");
                                    if (deal(deck, &dealer))
                                        swHandPrint(&dealer, true, "Dealer Hand:");
                                    else
                                    {
                                        failure = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    printf("Dealer choose STAND.\n");
                                }
                            }
                            if (dealer.value <= 21)
                            {
                                if (dealer.value > player.value)
                                    gameEnd(false, false, &player, &dealer);
                                else if (dealer.value < player.value)
                                    gameEnd(true, false, &player, &dealer);
                                else
                                    gameEnd(false, true, &player, &dealer);
                            }
                            else
                                gameEnd(true, false, &player, &dealer);
                        }
                        else
                        {
                            if (playerAction == QUIT)
                                done = true;
                            break;
                        }
                        if (failure)
                            break;
                    }
                }
            }
            if (done)
                break;
        }
        swDeckDelete(deck);
    }
    return 0;
}