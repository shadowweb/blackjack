#include "deck.h"

uint32_t gamesCount = 0;
uint32_t playerWinCount = 0;
uint32_t playerDrawCount = 0;
double playerPercentageWin = 0.0;

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

int main(int argc, char *argv)
{
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
                if (deal(deck, &player) && deal(deck, &dealer) && deal(deck, &player) && deal(deck, &dealer))
                {
                    swHandPrint(&player, true);
                    swHandPrint(&dealer, false);
                    // get next action in while loop
                    // if action == hit, deal another card to the player and quit the while loop if the value is greater tham 21
                    // if action == stand, quit the loop
                    // if action == quit, set done to true and quite while loop
                    // if not done or the player have not lost or has draw
                    // take dealer action in the while loop
                    // if value < 17 or == 17 and one of the cards is ace, hit; otherwise stand and quit while loop
                    // if dealer value > 21, then dealer lost; exit loop
                    // outside the loop, if none of the hands has lost yet, compare values and determine the looser
                }
            }
            if (done)
                break;
        }
        swDeckDelete(deck);
    }
    return 0;
}