CC=gcc

all: main.c
	$(CC) -g main.c render.c -o main

run: all
	./main

val: all
	valgrind -s ./main
