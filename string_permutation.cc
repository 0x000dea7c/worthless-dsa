#include <string>
#include <utility>
#include <tuple>
#include <iostream>
#include <type_traits>
#include <cstdint>
#include <stack>

using u32 = std::uint32_t;

template<typename T>
concept std_string = std::is_same_v<T, std::string>;

// this version's pretty good: ~100 lines of assembly aprox (counting std::cout)
void
permute (std_string auto& str, u32 start, u32 end)
{
  if (start == end)
    {
      std::cout << str << '\n';
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
}

// TODO: iterative approach that doesn't suck.

int
main ()
{
  using namespace std::string_literals;

  std::string str {"dog"s};

  permute (str, 0, str.length () - 1);

  return EXIT_SUCCESS;
}
