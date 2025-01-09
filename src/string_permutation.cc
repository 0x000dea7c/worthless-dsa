#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

static void
permute(std::string &str, int start, int end, std::vector<std::string> &res)
{
  if (start == end)
    {
      res.push_back(str);
    }
  else
    {
      for (int i = start; i <= end; ++i)
        {
          std::swap (str[start], str[i]);
          permute (str, start + 1, end, res);
          std::swap (str[start], str[i]);
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

  assert (permutations[0] == "dgo"s);
  assert (permutations[1] == "dog"s);
  assert (permutations[2] == "gdo"s);
  assert (permutations[3] == "god"s);
  assert (permutations[4] == "odg"s);
  assert (permutations[5] == "ogd"s);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
