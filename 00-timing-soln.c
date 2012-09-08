#include <stdio.h>
#include "timing.h"


int main()
{
  timestamp_type time1, time2;
  get_timestamp(&time1);

  long int temp = 1;
  for (int i = 0; i< 250000000; i++)
    temp+= 3*temp-1;

  get_timestamp(&time2);
  printf("that took %f seconds\n",
      timestamp_diff_in_seconds(time1,time2));

  // printf("and the result was %d\n", temp);

  return 0;
}
