#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <cassert>

// PARTITION, RECURSE!
template <typename T>
int partition(std::vector<T> &data, int start, int end)
{
  auto pivot = data[end];
  int p = start - 1;
  for (int i = start; i < end; ++i) {
    if (data[i] < pivot) {
      ++p;
      std::swap(data[p], data[i]);
    }
  }
  ++p;
  std::swap(data[end], data[p]);
  return p;
}

template <typename T>
void quicksort(std::vector<T> &data, int start, int end)
{
  if (start < end) {
    int p = partition(data, start, end);
    quicksort(data, start, p - 1);
    quicksort(data, p + 1, end);
  }
}

template <typename T>
void
sort (std::vector<T> &data)
{
  if (data.empty () || data.size () == 1)
    {
      return;
    }
  quicksort (data, 0, static_cast<int> (data.size () - 1));
}

int
main ()
{
  using namespace std::string_literals;

  {
    // Already sorted, shouldn't do anything.
    std::vector<std::string> data{"alligator"s, "befriend"s, "colour"s};

    sort (data);

    assert (data[0] == "alligator"s);
    assert (data[1] == "befriend"s);
    assert (data[2] == "colour"s);
  }

  {
    // Worst case scenario.
    std::vector<std::string> data{
      "zhang"s, "yoshika"s, "xiye"s,   "wai"s,  "victoria"s, "terry"s, "simon",  "rose"s,
      "qing"s,  "petra"s,   "oliver"s, "nana"s, "maria"s,    "lucas"s, "keiko"s,
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
    std::vector<std::string> data{"here", "we", "go", "even", "number", "of", "elements", "here"};

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
    std::vector<std::string> data{
      "here", "we", "go", "even", "number", "of", "elements",
    };

    sort (data);

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
