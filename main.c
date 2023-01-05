/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#include "mandelbrot.h"

int main() {
  mRun("serial", false);
  mRun("parallel", true);
  return 0;
}