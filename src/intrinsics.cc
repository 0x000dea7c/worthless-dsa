#include <cstdlib>
#include <chrono>
#include <immintrin.h>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <cmath>
#include "immintrin.h"

static uint64_t constexpr expected{5'000'050'000};
static double constexpr expected_flt{5000050000.0};

// NOTE: VERY IMPORTANT!! using float is a nono bc precision issues, it gets nasty as we keep adding,
// so using double instead!!
//
// NOTE: i tried to use 32 byte aligned data to then use the aligned versions of load and store to hope
// and get huge gainz, but nah, same speed p much
//
// conclusion: it's better to use vector and unaligned func versions to start with. i think to get even
// more speed than this i'll probably need to know more in depth stuff, it's not as simple
//
static bool
are_equal (double a, double b, double tolerance = std::numeric_limits<double>::epsilon ())
{
  return std::fabs (a - b) <= tolerance;
}

// VERY SIMPLE AND NAIVE!!!!
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
sum (T *data, size_t n)
{
  // initialise sum vector
  __m256i sum_vector = _mm256_setzero_si256 ();
  size_t i = 0;
  auto now = std::chrono::high_resolution_clock::now ();
  // process in chunks of 4 uint64_t (8 * 4 * 8 -> 256 bits)
  for (; i + 4 <= n; i += 4)
    {
      __m256i data_vector = _mm256_load_si256 (reinterpret_cast<__m256i const *> (&data[i]));
      sum_vector = _mm256_add_epi64 (sum_vector, data_vector);
    }
  // horizontal sum of the avx2 vector
  uint64_t temp[4];
  _mm256_store_si256 (reinterpret_cast<__m256i *> (temp), sum_vector);
  uint64_t total_sum = 0;
  // this shit is gonna be fast, prolly unrolled
  for (int j = 0; j < 4; ++j)
    {
      total_sum += temp[j];
    }
  // leftovers bc n is not a multiple of 4
  for (; i < n; ++i)
    {
      total_sum += data[i];
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "Total sum -> " << total_sum << " in "
            << std::chrono::duration_cast<std::chrono::microseconds> (end - now).count ()
            << " us for integer version\n";
  assert (total_sum == expected);
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value && sizeof (T) == 8, void>::type
sum (T *data, size_t n)
{
  __m256d sum_vector = _mm256_setzero_pd ();
  auto now = std::chrono::high_resolution_clock::now ();
  // process in chunks of 4 doubles (4 * 8 * 8 = 256 bits)
  size_t i = 0;
  for (; i + 4 <= n; i += 4)
    {
      __m256d data_vector = _mm256_load_pd (&data[i]);
      sum_vector = _mm256_add_pd (sum_vector, data_vector);
    }
  double temp[4];
  _mm256_store_pd (temp, sum_vector);
  double total_sum = 0.0;
  for (int j = 0; j < 4; ++j)
    {
      total_sum += temp[j];
    }
  for (; i < n; ++i)
    {
      total_sum += data[i];
    }
  auto end = std::chrono::high_resolution_clock::now ();
  std::cout << "Total sum -> " << std::fixed << total_sum << " in "
            << std::chrono::duration_cast<std::chrono::microseconds> (end - now).count () << " us for double version\n";
  assert (are_equal (total_sum, expected_flt));
}

int
main ()
{
  auto const n = 100'000 + 1;
  uint64_t *data_int = new (std::align_val_t (32)) uint64_t[n];
  double *data_dbl = new (std::align_val_t (32)) double[n];
  for (size_t i = 0; i < n; ++i)
    {
      data_int[i] = i;
      data_dbl[i] = static_cast<double> (i);
    }
  sum (data_int, n);
  sum (data_dbl, n);
  // voodoo black magic to force 32 byte alignment and get max speedo
  ::operator delete[] (data_int, std::align_val_t (32));
  ::operator delete[] (data_dbl, std::align_val_t (32));
  return EXIT_SUCCESS;
}
