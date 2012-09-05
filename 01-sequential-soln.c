#include "timing.h"
#include <stdio.h>
#include <stdlib.h>

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

  struct timespec time1, time2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

  for (int trip = 0; trip < ntrips; ++trip)
  {
    for (int i = 0; i < n; ++i)
    {
      z[i] = x[i] + y[i];
    }
  }

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  double elapsed = timespec_diff_in_seconds(time1,time2)/ntrips;
  printf("%f GB/s\n",
      3*n*sizeof(double)/1e9/elapsed);
  printf("%f GFlops/s\n",
      n/1e9/elapsed);

  return 0;
}
