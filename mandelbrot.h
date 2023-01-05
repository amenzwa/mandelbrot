/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "complex.h"

#define D 0.001 // c-plane delta
#define R 10.0 // escape radius
#define I 100 // maximum number of iterations
#define L 255 // gray scale maximum level

typedef struct Mandelbrot {
  Complex tl, br; // top-left and bottom-right corners
  int w, h; // image width and height
  int **i; // grayscale image
  bool invert; // invert grayscale flag
} Mandelbrot;

extern Mandelbrot* mNew(Complex tl, Complex br, bool inv);
extern void mDel(Mandelbrot* m);
extern void mRun(const char* s, bool par);

#endif //MANDELBROT_MANDELBROT_H
