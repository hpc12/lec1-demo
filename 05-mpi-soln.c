#include "timing.h"
#include <stdio.h>
#include <stdlib.h>
// FIXME kill
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank_count, my_rank, worker_count;
  // FIXME kill
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &rank_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  worker_count = rank_count-1;

  if (argc != 3)
  {
    fprintf(stderr, "need two arguments!\n");
    abort();
  }

  const long n = atol(argv[1]);
  const int ntrips = atoi(argv[2]);

  // FIXME kill
  if (n % worker_count != 0)
  {
    fprintf(stderr, "size not divisible\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  long divided_n = n / worker_count;

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

    for (int i = 0; i < worker_count; ++i)
    {
      printf("before send %d\n", i);
      MPI_Send(x + i*divided_n, divided_n, MPI_DOUBLE, i+1, tag, MPI_COMM_WORLD);
      MPI_Send(y + i*divided_n, divided_n, MPI_DOUBLE, i+1, tag, MPI_COMM_WORLD);
      printf("after send %d\n", i);
    }
    for (int i = 0; i < worker_count; ++i)
    {
      printf("before recv %d\n", i);
      MPI_Recv(z + i*divided_n, divided_n, MPI_DOUBLE, i+1, tag, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
      printf("after recv %d\n", i);
    }

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

    MPI_Recv(xbuf, divided_n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
    MPI_Recv(ybuf, divided_n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);

    printf("start rank %d\n", my_rank);
    for (int trip = 0; trip < ntrips; ++trip)
    {
      for (int i = 0; i < divided_n; ++i)
      {
        zbuf[i] = xbuf[i] + ybuf[i];
      }
    }
    printf("done rank %d\n", my_rank);

    MPI_Send(zbuf, divided_n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    printf("done rank %d\n", my_rank);
  }

  MPI_Finalize();

  return 0;
}
