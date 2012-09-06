#include <time.h>

#ifdef __APPLE___

typedef struct timeval timestamp_type;

static void get_current_time(timestamp_type *t)
{
  gettimeofday(t);
}

double
timeval_subtract (timestamp_type x, timestamp_type y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x.tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x.tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x.tv_usec - y->tv_usec > 1000000) {
    int nsec = (x.tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x.tv_sec;
  result->tv_usec = x.tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x.tv_sec < y->tv_sec;
}

#else

typedef struct timespec timestamp_type;

static void get_current_time(timestamp_type *t)
{
  clock_gettime(CLOCK_REALTIME, t);
}

static double timespec_diff_in_seconds(timestamp_type start, timestamp_type end)
{
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp.tv_sec + 1e-9*temp.tv_nsec;
}

#endif
