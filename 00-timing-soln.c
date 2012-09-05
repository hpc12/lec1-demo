#include <stdio.h>
#include <time.h>

double timespec_diff_in_seconds(struct timespec start, struct timespec end)
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

int main()
{
  struct timespec time1, time2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

  long int temp = 1;
  for (int i = 0; i< 250000000; i++)
    temp+= 3*temp-1;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  printf("that took %f seconds\n",
      timespec_diff_in_seconds(time1,time2));

  // printf("and the result was %d\n", temp);

  return 0;
}
