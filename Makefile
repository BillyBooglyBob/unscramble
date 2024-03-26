CC=gcc
CFLAGS= -Wextra -Wall -pedantic -std=gnu99

all: unscramble

unscramble: main.c
	$(CC) $(CFLAGS) $^ -g -o $@



