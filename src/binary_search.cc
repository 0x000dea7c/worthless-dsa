#include <cassert>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>

// NOTE: big assumption: in order for binary search to work, the container must be
// sorted beforehand!!!
template<typename T>
bool
binary_search (std::vector<T>& data, T&& value)
{
  auto const n = data.size ();
  int32_t left = 0, right = n - 1;
  int32_t mid;

  while (left <= right)
    {
      mid = left + ((right - left) / 2);

      if (value > data[mid])
        {
          left = mid + 1;
        }
      else if (value < data[mid])
        {
          right = mid - 1;
        }
      else
        {
          return true;
        }
    }

  return false;
}

int
main ()
{
  using namespace std::string_literals;

  {
    std::vector<std::string> data {
      "windows"s, "is"s, "evil"s
    };

    // REMEMBER TO SORT BEFORE!
    std::sort (data.begin (), data.end ());

    assert (binary_search (data, "windows"s));
    assert (binary_search (data, "is"s));
    assert (binary_search (data, "evil"s));

    assert (! binary_search (data, "england"s));
  }

  {
    std::vector<uint32_t> data;

    for (uint32_t i = 0; i < 100'000; ++i)
      {
        data.push_back (i);
      }

    // This should be fast since this takes O(log n) time.
    assert (binary_search (data, 50'000U));
    assert (binary_search (data, 12'373U));
    assert (binary_search (data, 2'121U));
    assert (binary_search (data, 98'153U));
    assert (binary_search (data, 77'551U));

    assert (! binary_search (data, 100'001U));
  }

  {
    // Empty container.
    std::vector<uint32_t> data;
    assert (! binary_search (data, 100U));
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
