#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <cstdint>

// // SSS (select, swap, step forward)
template <typename T>
void
selection_sort (std::vector<T> &data)
{
  auto const n = data.size ();
  for (size_t i = 0; i < n; ++i)
    {
      size_t j_min = i;
      for (size_t j = i + 1; j < n; ++j)
        {
          if (data[j] < data[j_min])
            {
              j_min = j;
            }
        }
      if (i != j_min)
        {
          std::swap (data[i], data[j_min]);
        }
    }
}

template <typename T>
void
sort (std::vector<T> &data)
{
  selection_sort (data);
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
