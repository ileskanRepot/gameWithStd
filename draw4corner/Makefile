CC=gcc
FILES=render.c misc.c inputs.c physics.c

all: main.c $(FILES)
	$(CC) -g -lm -O3 -fopenmp main.c $(FILES) -o main

test: test.c $(FILES)
	$(CC) -g -lm -O3 -fopenmp test.c $(FILES) -o test

run: all
	sudo ./main

val: all
	valgrind -s ./main
