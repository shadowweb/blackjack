CC =    cc
CFLAGS =  -pipe -O -W -Wall -Winline -Wpointer-arith -Wno-unused-parameter -Werror -g -std=c11 -funit-at-a-time
CPP =   cc -E
LINK =  $(CC)

all: game

game: game.o deck.o
	$(CC) -o $@ $^

game.o: game.c
	$(CC) -c $(CFLAGS) $(ALL_INCS) -o $@ $<

deck.o: deck.c deck.h
	$(CC) -c $(CFLAGS) $(ALL_INCS) -o $@ $<
