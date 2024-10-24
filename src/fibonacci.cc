#include <cassert>
#include <vector>
#include <cstdlib>
#include <iostream>

using uint128_t = __uint128_t;

static uint128_t
fibonacci_top_down (uint128_t n, std::vector<uint128_t>& cache)
{
  if (n == 0 || n == 1)
    {
      return n;
    }

  if (cache[n] != 0)
    {
      return cache[n];
    }

  cache[n] = fibonacci_top_down (n - 2, cache) + fibonacci_top_down (n - 1, cache);

  return cache[n];
}

static uint128_t
fibonacci_bottom_up (uint128_t n)
{
  if (n == 0 || n == 1)
    {
      return n;
    }

  std::vector<uint128_t> data (n + 1, 0);

  data[0] = 0;
  data[1] = 1;

  for (uint128_t i = 2; i <= n; ++i)
    {
      data[i] = data[i - 2] + data[i - 1];
    }

  return data[n];
}

int
main ()
{
  std::vector<uint128_t> cache (30, 0);

  auto top_down  = fibonacci_top_down (cache.size () - 1, cache);
  auto bottom_up = fibonacci_bottom_up (cache.size () - 1);

  assert (top_down  == 514229);
  assert (bottom_up == 514229);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
