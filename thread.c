/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#include <assert.h>
#include "thread.h"

static int n = 0; // number of active threads

Thread tRun(Worker w, void* data) {
  assert(n < NUM_THREADS);
  pthread_t t;
  int e = pthread_create(&t, NULL, w, data);
  assert(e == 0);
  n++;
  return (Thread){t, Running};
}

void tStop(Thread t) {
  int e = pthread_join(t.t, NULL);
  assert(e == 0);
  t.s = Idle;
  n--;
}