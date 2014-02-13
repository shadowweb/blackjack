#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#include "deck.h"

uint32_t gamesCount = 0;
uint32_t playerWinCount = 0;
uint32_t playerDrawCount = 0;
double playerPercentageWin = 0.0;

void gameEnd(bool playerWin, bool draw, swHand *playerHand, swHand *dealerHand)
{
    gamesCount++;
    if (playerWin)
        playerWinCount++;
    if (draw)
        playerDrawCount++;
    playerPercentageWin = ((double)playerWinCount * 100.0)/(double)gamesCount;
    printf ("*** Game End: %s; Player Percentage Win: %f%%\n", ((playerWin)? "Player Won" : ((draw)? "Draw" : "Dealer Won" ) ), playerPercentageWin);
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

typedef enum swAction
{
    NONE,
    HIT,
    STAND,
    QUIT
} swAction;

// here I did not want to use getch and see echo and have to press enter for input
// I used the solution I found on stack overflow web site:
// http://stackoverflow.com/questions/421860/c-c-capture-characters-from-standard-input-without-waiting-for-enter-to-be-pr
// this way I can change the way the terminal bahaves while I read the character in
char nextChar()
{
    char buf = 0;
    struct termios oldTerm = {0};
    struct termios newTerm = {0};

    if (tcgetattr(0, &oldTerm) < 0)
            perror("tcsetattr()");
    newTerm = oldTerm;
    newTerm.c_lflag &= ~ICANON;
    newTerm.c_lflag &= ~ECHO;
    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &newTerm) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    if (tcsetattr(0, TCSADRAIN, &oldTerm) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

// press 's' for HIT or 'h' for STAND; anything else is end of the game
swAction getNextAction()
{
    swAction rtn = NONE;
    char actionChar;
    if ((actionChar = nextChar()) > 0)
    {
        if (actionChar == 'h')
            rtn = HIT;
        else if (actionChar == 's')
            rtn = STAND;
        else
            rtn = QUIT;
    }
    return rtn;
}

int main(int argc, char *argv[])
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
                printf ("\n*** Game %u started\n", (i+1));
                swHand player = {{NULL}, 0, 0, false};
                swHand dealer = {{NULL}, 0, 0, false};
                bool failure = false;
                // deal 4 cards
                if (deal(deck, &player) && deal(deck, &dealer) && deal(deck, &player) && deal(deck, &dealer))
                {
                    swHandPrint(&player, true, "Player Hand:");
                    swHandPrint(&dealer, false, "Dealer Hand:");

                    // see if the player has won or there is a draw
                    if (player.value == 21)
                    {
                        swHandPrint(&dealer, true, "Dealer Hand:");
                        if (dealer.value == 21)
                            gameEnd(false, true, &player, &dealer);
                        else
                            gameEnd(true, false, &player, &dealer);
                    }
                    else
                    {
                        // player moves
                        swAction playerAction = NONE;
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
                        {
                            swHandPrint(&dealer, true, "Dealer Hand:");
                            gameEnd(false, false, &player, &dealer);
                        }
                        else if (playerAction == STAND)
                        {
                            // dealer moves
                            printf("Player choose STAND.\n");
                            swHandPrint(&dealer, true, "Dealer Hand:");
                            swAction dealerAction = HIT;
                            while (dealerAction == HIT)
                            {
                                if ((dealer.value < 17) || ((dealer.value == 17) && dealer.hasAce))
                                {
                                    printf("Dealer choose HIT.\n");
                                    if (deal(deck, &dealer))
                                        swHandPrint(&dealer, true, "Dealer Hand:");
                                    else
                                    {
                                        dealerAction = NONE;
                                        failure = true;
                                    }
                                }
                                else
                                {
                                    dealerAction = STAND;
                                    printf("Dealer choose STAND.\n");
                                }
                            }
                            if (dealerAction == STAND)
                            {
                                // decide who won
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
                        }
                        else
                        {
                            if (playerAction == QUIT)
                                done = true;
                        }
                    }
                }
                else
                    failure = true;
                if (failure || done)
                    break;
            }
            if (done)
                break;
        }
        swDeckDelete(deck);
    }
    return 0;
}
