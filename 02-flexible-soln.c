#include "timing.h"
#include <stdio.h>
#include <stdlib.h>

extern void add_doubles(void *tgt, void *op1, void *op2);

typedef void (*operation_t)(void *, void *, void *);

void do_three_operand_loop(operation_t op, void *x, void *y, void *z,
    long item_size, long n)
{
  for (int i = 0; i < n; ++i)
  {
    op(z+i*item_size, x+i*item_size, y+i*item_size);
  }
}

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

  timestamp_type time1, time2;
  get_timestamp(&time1);

  for (int trip = 0; trip < ntrips; ++trip)
  {
    do_three_operand_loop(add_doubles, x, y, z, sizeof(double), n);
  }

  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2)/ntrips;
  printf("%f GB/s\n",
      3*n*sizeof(double)/1e9/elapsed);
  printf("%f GFlops/s\n",
      n/1e9/elapsed);

  return 0;
}
