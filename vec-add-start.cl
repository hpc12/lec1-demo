__kernel void sum(
    __global const double *a,
    __global const double *b, 
    __global double *c,
    long n)
{
  for (long i = 0; i < n; ++i)
  {
    c[i] = a[i] + b[i];
  }
}
