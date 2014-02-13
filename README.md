Assignment
==========

Please implement a command-line blackjack game using C, C++, or Objective-C:

Requirements

•        Dealer must hit on soft 17
•        Single Deck. The deck is shuffled every 6 rounds.
•        Player is not allowed to split cards.
•        Keep track of win percentage for the player.
•        Provide a readme file explaining how to compile the source and other info that might be interesting.
 
You don't have to implement any AI other than the one mentioned above - it's just one player vs. dealer.

Design
======

The game is designed to be played in terminal window. Since I am not a card player I used the rule set I found on this website:

http://www.pagat.com/banking/blackjack.html

and whatever the additional constraints I could deduce from the problem statement.

Build
=====

To build use a simple "make" command.

Play
====

To play type:

./game

When it is player turn type 'h' for HIT or 's' for STAND. Any other character will terminate the game.

