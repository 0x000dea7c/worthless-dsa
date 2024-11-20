#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

void
cross_off (uint32_t prime, std::vector<bool> &primes)
{
  for (uint32_t i = prime * prime; i < primes.size (); i += prime)
    {
      primes[i] = false;
    }
}

uint32_t
get_next_prime (uint32_t prime, std::vector<bool> &primes)
{
  ++prime;
  while (prime < primes.size () && !primes[prime])
    {
      ++prime;
    }
  return prime;
}

std::vector<bool>
sieve_of_eratosthenes (uint32_t max)
{
  std::vector<bool> primes (max + 1, true);
  primes[0] = primes[1] = primes[2] = false;
  uint32_t prime = 2;
  while (prime <= sqrt (max))
    {
      cross_off (prime, primes);
      prime = get_next_prime (prime, primes);
    }
  return primes;
}

int
main ()
{
  std::vector<uint32_t> primes{2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37, 41,
                               43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

  auto result = sieve_of_eratosthenes (100);

  for (uint32_t i = 0; i < result.size (); ++i)
    {
      if (result[i])
        {
          assert (std::find (primes.begin (), primes.end (), i) != primes.end ());
        }
    }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
