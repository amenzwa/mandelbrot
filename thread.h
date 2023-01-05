/* Author: Amen Zwa, Esq.
 * Copyright (c) 2022 sOnit, Inc. */

#ifndef MANDELBROT_THREAD_H
#define MANDELBROT_THREAD_H

#include <pthread/pthread.h>

#define NUM_THREADS 100 // maximum number of threads (system dependent)

typedef enum ThreadStatus {
  Running, Idle
} ThreadStatus;

typedef struct Thread {
  pthread_t t;
  ThreadStatus s;
} Thread;

typedef void* (*Worker)(void* data);

extern Thread tRun(Worker w, void* data);
extern void tStop(Thread t);

#endif //MANDELBROT_THREAD_H
