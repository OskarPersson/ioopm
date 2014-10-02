CC=gcc
FLAGS=-Wall -std=c99 -ggdb

main: main.o graph.o
	$(CC) $(FLAGS) main.o graph.o -o main
main.o: main.c graph.h
	$(CC) $(FLAGS) -c main.c
graph.o: graph.c
	$(CC) $(FLAGS) -c graph.c
test: test.c graph.o
	$(CC) $(FLAGS) -o test test.c graph.o
	./test
run:
	./main

.PHONY: test
