#include <cstdlib>
#include <chrono>
#include <immintrin.h>
#include <vector>
#include <cassert>
#include <iostream>
#include "immintrin.h"

static uint64_t constexpr expected{5'000'050'000};

int
main ()
{
  // sum 1M ones in a row using avx2
  std::vector<uint64_t> data (100'000 + 1);
  auto const n = data.size ();
  for (size_t i = 0; i < n; ++i)
    {
      data[i] = i;
    }
  // initialise sum vector
  __m256i sum_vector = _mm256_setzero_si256 ();
  size_t i = 0;
  auto now = std::chrono::high_resolution_clock::now ();
  // process in chunks of 4 uint64_t (8 * 4 * 8 -> 256 bits)
  for (; i + 4 <= data.size (); i += 4)
    {
      // this mf grabs 8 ints
      __m256i data_vector = _mm256_loadu_si256 (reinterpret_cast<__m256i const *> (&data[i]));
      sum_vector = _mm256_add_epi64 (sum_vector, data_vector);
    }
  // horizontal sum of the avx2 vector
  uint64_t temp[4];
  _mm256_storeu_si256 (reinterpret_cast<__m256i *> (temp), sum_vector);
  uint64_t total_sum = 0;
  // this shit is gonna be fast, prolly unrolled
  for (int j = 0; j < 4; ++j)
    {
      total_sum += temp[j];
    }
  // leftovers bc some elements might not fit into a full avx2 register
  for (; i < n; ++i)
    {
      total_sum += data[i];
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "Total sum -> " << total_sum << " in "
            << std::chrono::duration_cast<std::chrono::microseconds> (end - now).count () << " us\n";
  assert (total_sum == expected);
  return EXIT_SUCCESS;
}
