CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -O2
LIBS = -lglfw -lGL -lm -lGLU -lgmp

all: mcalc

mcalc: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f mcalc
