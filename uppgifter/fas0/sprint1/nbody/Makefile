# Make animate to show a nice window
# Make starsim to only benchmark

CC= gcc
CFLAGS= -std=c99 -Wall -ggdb

starsim:
	$(CC) $(CFLAGS) main.c -o main  -lm 
animate:
	$(CC) $(CFLAGS) -DANIMATE main.c -o main -lm -lX11 -L/usr/X11/lib -I/usr/X11/include
run:
	./main 200 10000
