#include "timing.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int rank_count, my_rank, worker_count;
  // FIXME init

  worker_count = rank_count-1;

  if (argc != 3)
  {
    fprintf(stderr, "need two arguments!\n");
    abort();
  }

  const long n = atol(argv[1]);
  const int ntrips = atoi(argv[2]);

  long divided_n = n / worker_count;
  // FIXME really?

  printf("rank %d/%d reporting for duty\n", my_rank, rank_count);

  const int tag = 0;
  if (my_rank == 0)
  {
    printf("doing %d trips...\n", ntrips);

    double *x = (double *) malloc(sizeof(double) * n);
    if (!x) { perror("alloc x"); MPI_Abort(MPI_COMM_WORLD, 1); }
    double *y = (double *) malloc(sizeof(double) * n);
    if (!y) { perror("alloc y"); MPI_Abort(MPI_COMM_WORLD, 1); }
    double *z = (double *) malloc(sizeof(double) * n);
    if (!z) { perror("alloc z"); MPI_Abort(MPI_COMM_WORLD, 1); }

    for (int i = 0; i < n; ++i)
    {
      x[i] = i;
      y[i] = 2*i;
    }

    struct timespec time1, time2;
    clock_gettime(CLOCK_REALTIME, &time1);

    // FIXME send / receive

    clock_gettime(CLOCK_REALTIME, &time2);
    double elapsed = timespec_diff_in_seconds(time1,time2)/ntrips;
    printf("%f GB/s\n",
        3*n*sizeof(double)/1e9/elapsed);
    printf("%f GFlops/s\n",
        n/1e9/elapsed);

    for (int i = 0; i < n; ++i)
    {
      if (z[i] != x[i] + y[i])
      {
        printf("bad %d\n", i);
        MPI_Abort(MPI_COMM_WORLD, 1);
      }
    }
  }
  else
  {
    double *xbuf = (double *) malloc(sizeof(double) * divided_n);
    if (!xbuf) { perror("alloc xbuf"); MPI_Abort(MPI_COMM_WORLD, 1); }
    double *ybuf = (double *) malloc(sizeof(double) * divided_n);
    if (!ybuf) { perror("alloc ybuf"); MPI_Abort(MPI_COMM_WORLD, 1); }
    double *zbuf = (double *) malloc(sizeof(double) * divided_n);
    if (!zbuf) { perror("alloc zbuf"); MPI_Abort(MPI_COMM_WORLD, 1); }

    // FIXME recv

    for (int trip = 0; trip < ntrips; ++trip)
    {
      for (int i = 0; i < divided_n; ++i)
      {
        zbuf[i] = xbuf[i] + ybuf[i];
      }
    }

    // FIXME send
  }

  // FIXME finalize
  MPI_Finalize();

  return 0;
}
