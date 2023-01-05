/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#ifndef COMPLEX_COMPLEX_H
#define COMPLEX_COMPLEX_H

#include <stdbool.h>

typedef enum ComplexKind {
  R, P // rectangular | polar
} ComplexKind;

typedef struct Complex {
  ComplexKind k;
  double a, b; // a+jb | |a|/_b
} Complex;

extern Complex rOfD(double x, double y);
extern Complex rOfP(Complex p);
extern Complex pOfD(double m, double g);
extern Complex pOfR(Complex r);
extern double cMod(Complex c);
extern double cArg(Complex c);
extern Complex cConj(Complex c);
extern Complex cAdd(Complex c, Complex d);
extern Complex cSub(Complex c, Complex d);
extern Complex cMul(Complex c, Complex d);
extern Complex cDiv(Complex c, Complex d);
extern Complex cSqre(Complex c);
extern bool isR(Complex r);
extern bool isP(Complex p);

#endif //COMPLEX_COMPLEX_H
