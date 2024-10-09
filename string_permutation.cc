#include <string>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <cstdint>
#include <vector>
#include <algorithm>

using u32 = std::uint32_t;

template<typename T>
concept std_string = std::is_same_v<T, std::string>;

std::vector<std::string>
permute (std_string auto& str, u32 start, u32 end)
{
  static std::vector<std::string> permutations;

  if (start == end)
    {
      permutations.emplace_back (str);
    }
  else
    {
      for (u32 i {start}; i <= end; ++i)
	{
	  std::swap (str[start], str[i]);
	  permute (str, start + 1, end);
	  std::swap (str[start], str[i]);
	}
    }

  return permutations;
}

// this version's pretty good: ~100 lines of assembly aprox (counting std::cout)
// void
// permute (std_string auto& str, u32 start, u32 end)
// {
//   if (start == end)
//     {
//       std::cout << str << '\n';
//     }
//   else
//     {
//       for (u32 i {start}; i <= end; ++i)
// 	{
// 	  std::swap (str[start], str[i]);
// 	  permute (str, start + 1, end);
// 	  std::swap (str[start], str[i]);
// 	}
//     }
// }

// TODO: iterative approach that doesn't suck.

int
main ()
{
  using namespace std::string_literals;

  std::string str {"dog"s};

  auto results = permute (str, 0, str.length () - 1);

  std::sort (results.begin (), results.end ());

  assert (results[0] == "dgo");
  assert (results[1] == "dog");
  assert (results[2] == "gdo");
  assert (results[3] == "god");
  assert (results[4] == "odg");
  assert (results[5] == "ogd");

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
