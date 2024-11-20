#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <thread>
#include <cstdint>

void
do_sum (uint32_t start, uint32_t end, uint32_t thread_id, std::vector<uint32_t> &data, std::vector<uint32_t> &results)
{
  uint32_t result = 0;
  for (uint32_t i = start; i <= end; ++i)
    {
      result += data[i];
    }
  results[thread_id] = result;
}

int
main ()
{
  uint32_t constexpr expected{50'005'000};
  std::vector<uint32_t> data (10'000 + 1);
  for (uint32_t i{0}; i < data.size (); ++i)
    {
      data[i] = i;
    }
  uint32_t num_threads = std::min<uint32_t> (6, std::thread::hardware_concurrency ());
  std::vector<std::thread> pool;
  uint32_t chunk_size = data.size () / num_threads;
  std::vector<uint32_t> results (num_threads, 0);
  uint32_t start = 0;
  for (uint32_t thread_idx = 0; thread_idx < num_threads; ++thread_idx)
    {
      if (thread_idx == num_threads - 1)
        {
          pool.emplace_back (&do_sum, start, data.size () - 1, thread_idx, std::ref (data), std::ref (results));
        }
      else
        {
          pool.emplace_back (&do_sum, start, start + chunk_size, thread_idx, std::ref (data), std::ref (results));
        }
      start = start + chunk_size + 1;
    }
  for (auto &thread : pool)
    {
      thread.join ();
    }
  uint32_t result = 0;
  for (auto const &res : results)
    {
      result += res;
    }
  std::cout << "Result was: " << result << '\n';
  assert (result == expected);
  return EXIT_SUCCESS;
}
