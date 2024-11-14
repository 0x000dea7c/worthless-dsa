#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

void
permute (std::string &word, size_t start, size_t end, std::vector<std::string> &results)
{
  if (start == end)
    {
      results.push_back (word);
    }
  else
    {
      for (size_t i = start; i <= end; ++i)
        {
          std::swap (word[i], word[start]);
          permute (word, start + 1, end, results);
          std::swap (word[i], word[start]);
        }
    }
}

int
main ()
{
  using namespace std::string_literals;

  std::string str{"dog"s};
  std::vector<std::string> permutations;

  permute (str, 0, str.length () - 1, permutations);

  std::sort (permutations.begin (), permutations.end ());

  assert (permutations[0] == "dgo");
  assert (permutations[1] == "dog");
  assert (permutations[2] == "gdo");
  assert (permutations[3] == "god");
  assert (permutations[4] == "odg");
  assert (permutations[5] == "ogd");

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
