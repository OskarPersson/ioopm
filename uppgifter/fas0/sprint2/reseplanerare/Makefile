CC=gcc
FLAGS=-Wall -std=c99 -ggdb

main: main.o graph.o parse.o
	$(CC) $(FLAGS) main.o graph.o parse.o -o main
main.o: main.c graph.h parse.h
	$(CC) $(FLAGS) -c main.c
graph.o: graph.c
	$(CC) $(FLAGS) -c graph.c
test: test.c graph.o parse.o
	$(CC) $(FLAGS) -o test test.c graph.o parse.o -lcunit
	./test
parse.o: parse.c
	$(CC) $(FLAGS) -c parse.c
run:
	./main nätverk.txt

.PHONY: test
