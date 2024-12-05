#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

static size_t
get_next_prime (size_t prime, std::vector<bool> &crossed)
{
  prime = prime + 1;
  while (prime < crossed.size () && !crossed[prime])
    ++prime;
  return prime;
}

static void
cross_off (size_t prime, std::vector<bool> &crossed)
{
  for (auto i = prime * prime; i < crossed.size (); i += prime)
    crossed[i] = false;
}

std::vector<bool>
sieve_of_eratosthenes (size_t max)
{
  std::vector<bool> crossed (max + 1, true);
  crossed[0] = crossed[1] = false;
  size_t i = 2;
  while (i < std::sqrt (max))
    {
      cross_off (i, crossed);
      i = get_next_prime (i, crossed);
    }
  return crossed;
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
