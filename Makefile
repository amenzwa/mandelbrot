# Author: Amen Zwa, Esq.
# Copyright (c) 2022 sOnit, Inc.

CC=cc
CFLAGS=-std=c2x -O3 # -g

# utilities

thread.o:		thread.c thread.h
	${CC} ${CFLAGS} -c thread.c

complex.o:	complex.c complex.h
	${CC} ${CFLAGS} -c complex.c

# Mandelbrot

mandelbrot.o:	mandelbrot.c
	${CC} ${CFLAGS} -c mandelbrot.c

main.o: main.c
	${CC} ${CFLAGS} -c main.c

mandelbrot:	main.o mandelbrot.o complex.o thread.o
	${CC} ${CFLAGS} main.o mandelbrot.o complex.o thread.o -o mandelbrot

# miscellaneous

all:	mandelbrot

clean:
	rm -f *.o mandelbrot *.pgm