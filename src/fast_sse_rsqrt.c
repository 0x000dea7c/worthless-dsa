#include <stdio.h>

inline float
fast_sse_rsqrt (float n)
{
  float const three_halfs = 1.5f;
  float const n_half = (float) n * 0.5f;
  float rsqrt;

  __asm__ volatile ("rsqrtss %1, %%xmm0\n\t"
                    "movss %%xmm0, %0\n\t"
                    : "=m"(rsqrt)
                    : "m"(n)
                    : "xmm0");

  rsqrt = rsqrt * (three_halfs - (n_half * rsqrt * rsqrt));

  return rsqrt;
}

int
main (void)
{
  printf ("%.8f\n", fast_sse_rsqrt (9));
  return 0;
}
