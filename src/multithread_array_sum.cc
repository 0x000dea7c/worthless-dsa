#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <thread>
#include <cstdint>
#include <chrono>
#include <future>

// creating this mf in an attempt to reduce false sharing
struct padded_result
{
  padded_result () : _value{0} {}
  uint64_t _value;
  char _padding[64 - sizeof (uint64_t)];
};

void
do_sum (std::shared_future<void> start_signal, uint32_t start, uint32_t end, uint32_t thread_id,
        std::vector<uint64_t> &data, std::vector<padded_result> &results)
{
  start_signal.wait ();
  uint32_t result = 0;
  for (uint32_t i = start; i <= end; ++i)
    {
      result += data[i];
    }
  results[thread_id]._value = result;
}

static uint64_t constexpr expected{5'000'050'000};

int
main ()
{
  std::promise<void> start_promise;
  std::shared_future<void> start_signal = start_promise.get_future ();
  std::vector<uint64_t> data (100'000 + 1);
  auto const n = data.size ();
  for (uint32_t i{0}; i < n; ++i)
    {
      data[i] = i;
    }
  uint32_t num_threads = std::min<uint32_t> (4, std::thread::hardware_concurrency ());
  std::vector<std::thread> pool;
  uint32_t chunk_size = n / num_threads & ~3; // chunk_size is a mult of 4, try to vectorise this shit???
  std::vector<padded_result> results (num_threads);
  uint32_t start = 0;
  uint64_t final_result = 0;
  // fire these mfs, but don't start working yet bc i need to time them
  for (uint32_t thread_idx = 0; thread_idx < num_threads; ++thread_idx)
    {
      uint32_t end = (thread_idx == num_threads - 1) ? n - 1 : start + chunk_size - 1;
      pool.emplace_back (&do_sum, start_signal, start, end, thread_idx, std::ref (data), std::ref (results));
      start = end + 1;
    }
  auto start_time = std::chrono::high_resolution_clock::now ();
  start_promise.set_value (); // now work
  for (auto &thread : pool)
    {
      thread.join ();
    }
  for (auto const &res : results)
    {
      final_result += res._value;
    }
  auto end_time = std::chrono::high_resolution_clock::now ();
  auto total = end_time - start_time;
  std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds> (total).count () << "us\n";
  std::cout << "Result was: " << final_result << '\n';
  assert (final_result == expected);
  return EXIT_SUCCESS;
}
