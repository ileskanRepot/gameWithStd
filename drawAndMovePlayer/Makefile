CC=gcc

all: main.c
	$(CC) -g -lm main.c render.c inputs.c -o main

run: all
	./main

val: all
	valgrind -s ./main
