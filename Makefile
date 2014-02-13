all:
	game

game: game.o deck.o
    // link

game.o: game.c
    // compile

deck.o: deck.c deck.h
    // compile

