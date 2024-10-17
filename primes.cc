#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>

using u32 = std::uint32_t;

static void
cross_off (u32 prime, std::vector<bool>& primes)
{
  for (u32 i = prime * prime; i < primes.size (); i += prime)
    primes[i] = false;
}

static u32
get_next_prime (u32 prime, std::vector<bool>& primes)
{
  ++prime;

  while (prime < primes.size () && !primes[prime])
    ++prime;

  return prime;
}

std::vector<bool>
sieve_of_eratosthenes (u32 max)
{
  std::vector<bool> primes (max + 1, true);
  primes[0] = primes[1] = false;
  u32 prime = 2;
  u32 limit = std::sqrt (max);

  while (prime <= limit)
    {
      cross_off (prime, primes);
      prime = get_next_prime (prime, primes);
    }

  return primes;
}

int
main ()
{
  std::vector<u32> primes {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
    43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
  };

  auto result = sieve_of_eratosthenes (100);

  for (u32 i {}; i < result.size (); ++i)
    {
      if (result[i])
	{
	  assert (std::find (primes.begin (), primes.end (), i) != primes.end ());
	}
    }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
