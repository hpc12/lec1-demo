#include "timing.h"
#include <stdio.h>
#include <stdlib.h>
// FIXME STRIP
#include <omp.h>
#include <math.h>

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "need two arguments!\n");
    abort();
  }

  const long n = atol(argv[1]);
  double *x = (double *) malloc(sizeof(double) * n);
  if (!x) { perror("alloc x"); abort(); }
  double *y = (double *) malloc(sizeof(double) * n);
  if (!y) { perror("alloc y"); abort(); }
  double *z = (double *) malloc(sizeof(double) * n);
  if (!z) { perror("alloc z"); abort(); }

  for (int i = 0; i < n; ++i)
  {
    x[i] = i;
    y[i] = 2*i;
  }

  const int ntrips = atoi(argv[2]);
  printf("doing %d trips...\n", ntrips);

  // FIXME STRIP
#pragma omp parallel
  {
    printf("Hi, I'm thread # %d of %d\n",
        omp_get_thread_num(),
        omp_get_num_threads());
  }

  struct timespec time1, time2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

  // FIXME STRIP
  for (int trip = 0; trip < ntrips; ++trip)
  {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
      double a = x[i];
      double b = y[i];

      for (int j = 0; j < 200; ++j)
      {
        a = sin(a) + b;
        b = cos(b) + a;
      }

      z[i] = a+b;
    }
  }

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  double elapsed = timespec_diff_in_seconds(time1,time2)/ntrips;
  printf("%f s\n", elapsed);
  printf("%f GB/s\n",
      3*n*sizeof(double)/1e9/elapsed);
}

