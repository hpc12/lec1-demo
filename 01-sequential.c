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

  timestamp_type time1, time2;
  get_timestamp(&time1);

  // FIXME

  get_timestamp(&time2);
  printf("%f seconds for %d entries\n",
      timestamp_diff_in_seconds(time1, time2),
      n);

  return 0;
}
