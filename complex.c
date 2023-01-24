/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#include <assert.h>
#include <math.h>
#include "complex.h"

#define epsilon 1e-10

static inline double sqre(double x) {
  return x * x;
}

Complex rOfD(double x, double y) {
  /* Make a rectangular complex number from a pair of doubles. */
  return (Complex){R, x, y};
}

Complex rOfP(Complex p) {
  /* Make a rectangular complex number from a polar complex number. */
  assert(isP(p));
  return (Complex){R, p.a * cos(p.b), p.a * sin(p.b)};
}

Complex pOfD(double m, double g) {
  /* Make a polar complex number from a pair of doubles. */
  return (Complex){P, m, g};
}

Complex pOfR(Complex r) {
  /* Make a polar complex number from a rectangular complex number. */
  assert(isR(r));
  return (Complex){P, cMod(r), cArg(r)};
}

double cMod(Complex c) {
  if (isR(c)) return sqrt(sqre(c.a) + sqre(c.b));
  else return c.a;
}

double cArg(Complex c) {
  if (isR(c)) return fabs(c.a) <= epsilon ? (c.b < 0.0 ? 3.0 * M_PI_2 : M_PI_2) : atan(c.b / c.a);
  else return c.b;
}

Complex cConj(Complex c) {
  if (isP(c)) return pOfD(c.a, -c.b);
  else return rOfD(c.a, -c.b);
}

Complex cAdd(Complex c, Complex d) {
  if (isR(c) && isR(d)) return (Complex){R, c.a + d.a, c.b + d.b};
  else if (isP(c) && isP(d)) return pOfR(cAdd(rOfP(c), rOfP(d)));
  else assert(false);
}

Complex cSub(Complex c, Complex d) {
  if (isR(c) && isR(d)) return (Complex){R, c.a - d.a, c.b - d.b};
  else if (isP(c) && isP(d)) return pOfR(cSub(rOfP(c), rOfP(d)));
  else assert(false);
}

Complex cMul(Complex c, Complex d) {
  if (isP(c) && isP(d)) return (Complex){P, c.a * d.a, c.b + d.b};
  else if (isR(c) && isR(d)) return rOfP(cMul(pOfR(c), pOfR(d)));
  else assert(false);
}

Complex cDiv(Complex c, Complex d) {
  assert(fabs(d.a) > epsilon);
  if (isP(c) && isP(d)) return (Complex){P, c.a / d.a, c.b - d.b};
  else if (isR(c) && isR(d)) return rOfP(cDiv(pOfR(c), pOfR(d)));
  else assert(false);
}

Complex cSqre(Complex c) {
  if (isP(c)) return cMul(c, c);
  else {
    Complex p = pOfR(c);
    return rOfP(cMul(p, p));
  }
}

bool isR(Complex r) {
  return r.k == R;
}

bool isP(Complex p) {
  return p.k == P;
}