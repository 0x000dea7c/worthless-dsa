#include <cassert>
#include <vector>
#include <cstdlib>
#include <iostream>

using uint128_t = __uint128_t;

static uint128_t
fibonacci (uint128_t n, std::vector<uint128_t>& cache)
{
  if (n == 0 || n == 1)
    {
      return n;
    }

  if (cache[n] != 0)
    {
      return cache[n];
    }

  cache[n] = fibonacci (n - 2, cache) + fibonacci (n - 1, cache);

  return cache[n];
}

int
main ()
{
  std::vector<uint128_t> cache (30, 0);

  auto res = fibonacci (cache.size () - 1, cache);

  assert (res == 514229);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
