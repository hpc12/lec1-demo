#include "timing.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    // FIXME
  }

  const long n = atol(argv[1]);
  double *x = (double *) malloc(sizeof(double) * n);
  double *y = (double *) malloc(sizeof(double) * n);
  double *z = (double *) malloc(sizeof(double) * n);

  for (int i = 0; i < n; ++i)
  {
    x[i] = i;
    y[i] = 2*i;
  }

  struct timespec time1, time2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

  // FIXME

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  printf("%f seconds for %d entries\n",
      timespec_diff_in_seconds(time1,time2),
      n);
}

