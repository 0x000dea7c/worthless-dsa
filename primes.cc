#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>

using u32 = std::uint32_t;

static void
cross_off (u32 prime, std::vector<bool>& crossed)
{
  for (u32 i = prime * prime; i < crossed.size (); i += prime)
    crossed[i] = false;
}

static u32
get_next_prime (u32 prime, std::vector<bool>& crossed)
{
  u32 next = prime + 1;

  while (next < crossed.size () && !crossed[next])
    ++next;

  return next;
}

std::vector<bool>
sieve_of_eratosthenes (u32 max)
{
  std::vector<bool> crossed (max + 1, true);
  crossed[0] = crossed[1] = false; // 0 and 1 are not primes
  u32 prime = 2;

  while (prime <= std::sqrt (max))
    {
      cross_off (prime, crossed);
      prime = get_next_prime (prime, crossed);
    }

  return crossed;
}

int
main ()
{
  auto result = sieve_of_eratosthenes (100);

  for (u32 i {}; i < result.size (); ++i)
    if (result[i])
      std::cout << "Number " + std::to_string (i) + " is prime.\n";

  return EXIT_SUCCESS;
}
