#include <string>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <cstdint>
#include <vector>
#include <algorithm>

using u32 = std::uint32_t;

void
permute (std::string& str, u32 start, u32 end, std::vector<std::string>& permutations)
{
  if (start == end)
    {
      permutations.emplace_back (str);
    }
  else
    {
      for (u32 i {start}; i <= end; ++i)
	{
	  std::swap (str[start], str[i]);
	  permute (str, start + 1, end, permutations);
	  std::swap (str[start], str[i]);
	}
    }
}

int
main ()
{
  using namespace std::string_literals;

  std::string str {"dog"s};
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
