#include <iostream>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>

// DCC (divide, conquer, combine), O(n log n)
template<typename T>
void
merge (std::vector<T>& data, uint32_t start, uint32_t mid, uint32_t end)
{
  auto const n1 = mid - start + 1;
  auto const n2 = end - mid;

  std::vector<T> left_half (n1), right_half (n2);

  for (uint32_t i = 0; i < n1; ++i)
    {
      left_half[i] = std::move (data[start + i]);
    }

  for (uint32_t j = 0; j < n2; ++j)
    {
      right_half[j] = std::move (data[mid + j + 1]);
    }

  uint32_t i = 0, j = 0, k = start;

  while (i < n1 && j < n2)
    {
      if (left_half[i] <= right_half[j])
        {
          data[k++] = std::move (left_half[i++]);
        }
      else
        {
          data[k++] = std::move (right_half[j++]);
        }
    }

  while (i < n1)
    {
      data[k++] = std::move (left_half[i++]);
    }

  while (j < n2)
    {
      data[k++] = std::move (right_half[j++]);
    }
}

template<typename T>
void
mergesort (std::vector<T>& data, uint32_t start, uint32_t end)
{
  if (start < end)
    {
      uint32_t mid = start + ((end - start) / 2);

      mergesort (data, start, mid);
      mergesort (data, mid + 1, end);

      merge (data, start, mid, end);
    }
}

template<typename T>
void
sort (std::vector<T>& data)
{
  if (data.empty () || data.size () == 1)
    {
      return;
    }

  mergesort (data, 0, data.size () - 1);
}

int
main ()
{
  using namespace std::string_literals;

  {
    // Already sorted, shouldn't do anything.
    std::vector<std::string> data {"alligator"s, "befriend"s, "colour"s};

    sort (data);

    assert (data[0] == "alligator"s);
    assert (data[1] == "befriend"s);
    assert (data[2] == "colour"s);
  }

  {
    // Worst case scenario.
    std::vector<std::string> data {
      "zhang"s,
      "yoshika"s,
      "xiye"s,
      "wai"s,
      "victoria"s,
      "terry"s,
      "simon",
      "rose"s,
      "qing"s,
      "petra"s,
      "oliver"s,
      "nana"s,
      "maria"s,
      "lucas"s,
      "keiko"s,
    };

    sort (data);

    assert (data[0] == "keiko"s);
    assert (data[1] == "lucas"s);
    assert (data[2] == "maria"s);
    assert (data[3] == "nana"s);
    assert (data[4] == "oliver"s);
    assert (data[5] == "petra"s);
    assert (data[6] == "qing"s);
    assert (data[7] == "rose"s);
    assert (data[8] == "simon"s);
    assert (data[9] == "terry"s);
    assert (data[10] == "victoria"s);
    assert (data[11] == "wai"s);
    assert (data[12] == "xiye"s);
    assert (data[13] == "yoshika"s);
    assert (data[14] == "zhang"s);
  }

  {
    // Even number of elements.
    std::vector<std::string> data {
      "here", "we", "go", "even", "number", "of", "elements", "here"
    };

    sort (data);

    assert (data[0] == "elements"s);
    assert (data[1] == "even"s);
    assert (data[2] == "go"s);
    assert (data[3] == "here"s);
    assert (data[4] == "here"s);
    assert (data[5] == "number"s);
    assert (data[6] == "of"s);
    assert (data[7] == "we"s);
  }

  {
    // Odd number of elements.
    std::vector<std::string> data {
      "here", "we", "go", "even", "number", "of", "elements",
    };

    mergesort (data, 0, data.size () - 1);

    assert (data[0] == "elements"s);
    assert (data[1] == "even"s);
    assert (data[2] == "go"s);
    assert (data[3] == "here"s);
    assert (data[4] == "number"s);
    assert (data[5] == "of"s);
    assert (data[6] == "we"s);
  }

  {
    // Empty container. (doesn't blow up)
    std::vector<std::string> data;

    sort (data);

    assert (data.empty ());
  }

  {
    std::vector<int32_t> data;

    for (int32_t i = 10'000; i >= 0; --i)
      {
        data.push_back (i);
      }

    sort (data);

    for (int32_t i = 0; i <= 10'000; ++i)
      {
        assert (data[i] == i);
      }
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;

}
