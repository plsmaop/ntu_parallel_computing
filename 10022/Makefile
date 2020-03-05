CC=gcc
CFLAG=-std=c99 -O2 -Xpreprocessor -fopenmp -lomp -g -Wall 

all: matrix.h matrix.c main.c
	$(CC) $(CFLAG) -O2 matrix.c main.c -o matrix

.PHONY: clean
clean:
	rm -rf *.o
