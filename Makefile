CC=gcc
CFLAGS=$(shell pkg-config --cflags --libs ncurses)

snake: snake.o
	$(CC) -o snake snake.o $(CFLAGS)
snake.o: snake.c snake.h
	$(CC) -c snake.c $(CFLAGS)
clean:
	rm snake.o snake
