/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#include <printf.h>
#include <stdlib.h>
#include <math.h>
#include "thread.h"
#include "mandelbrot.h"

Mandelbrot* mNew(Complex tl, Complex br, bool invert) {
  Mandelbrot* m = malloc(sizeof(Mandelbrot));
  m->tl = tl;
  m->br = br;
  Complex d = cSub(m->tl, m->br);
  m->w = (int) (fabs(d.a) / D);
  m->h = (int) (fabs(d.b) / D);
  m->i = malloc(m->h * sizeof(int*));
  for (int y = 0; y < m->h; y++) m->i[y] = malloc(m->w * sizeof(int));
  m->invert = invert;
  return m;
}

void mDel(Mandelbrot* m) {
  for (int y = 0; y < m->h; y++) free(m->i[y]);
  free(m->i);
  m->i = NULL;
  free(m);
}

static void save(Mandelbrot* mm[], int n, int w, int h, const char* fn) {
  FILE* fp = fopen(fn, "wb");
  fprintf(fp, "P2\n%d %d\n%d\n", w, h, L);
  for (int i = 0; i < n; i++)
    for (int y = 0; y < mm[i]->h; y++) {
      for (int x = 0; x < w; x++) fprintf(fp, "%d ", mm[i]->i[y][x]);
      fprintf(fp, "\n");
    }
  fclose(fp);
}

static inline int gray(int i, bool inv) {
  /* Map iteration i from [0, I] to grayscale [0, 255]. */
  int g = i * L / I;
  return inv ? L - g : g;
}

static int iterate(Complex c) {
  /* Iterate z <- z^2 + c for the specified c. */
  Complex z = rOfD(0.0, 0.0);
  int i = 0;
  for (; i < I && cMod(z) < R; i++) z = cAdd(cSqre(z), c);
  return i;
}

static void* mandelbrot(void* data) {
  /* Compute the Mandelbrot set using the bounds specified in data. */
  Mandelbrot* m = (Mandelbrot*) data;
  int p = 0, q = 0;
  for (double y = m->tl.b; y >= m->br.b && q < m->h; y -= D, q++) {
    for (double x = m->tl.a; x <= m->br.a && p < m->w; x += D, p++) m->i[q][p] = gray(iterate(rOfD(x, y)), m->invert);
    p = 0; // reset to left edge
  }
  printf("  done [%+f|%+f] ~ [%+f|%+f]\n", m->tl.a, m->tl.b, m->br.a, m->br.b);
  return NULL;
}

static void serial() {
  /* Compute the Mandelbrot set using a single thread. */
  Mandelbrot* m = mNew(rOfD(-3.0, +2.0), rOfD(+1.0, -2.0), true);
  mandelbrot(m);
  Mandelbrot* mm[] = {m};
  save(mm, 1, m->w, m->h, "./mandelbrot-s.pgm");
  mDel(m);
}

static void parallel() {
  /* Compute the Mandelbrot set using multiple threads. */
  Thread tt[NUM_THREADS];
  Complex tl = rOfD(-3.0, +2.0);
  Complex br = rOfD(+1.0, -2.0);
  Complex d = cSub(tl, br); // c-plane dimensions
  double py = d.b / (double) NUM_THREADS; // c-plane patch height
  Mandelbrot* mm[NUM_THREADS];
  for (int t = 0; t < NUM_THREADS; t++) {
    Complex ptl = rOfD(tl.a, tl.b - (double) t * py);
    Complex pbr = rOfD(br.a, ptl.b - py);
    mm[t] = mNew(ptl, pbr, false);
    tt[t] = tRun(mandelbrot, mm[t]);
  }
  for (int t = 0; t < NUM_THREADS; t++) tStop(tt[t]);
  save(mm, NUM_THREADS, (int) (fabs(d.a) / D), (int) (fabs(d.b) / D), "./mandelbrot-p.pgm");
  for (int t = 0; t < NUM_THREADS; t++) mDel(mm[t]);
}

void mRun(const char* s, bool par) {
  printf("%s Mandelbrot begin\n", s);
  time_t bgn = time(NULL);
  if (par) parallel();
  else serial();
  time_t end = time(NULL);
  printf("%s Mandelbrot end (%ld s)\n", s, end - bgn);
}