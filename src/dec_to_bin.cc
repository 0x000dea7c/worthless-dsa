#include <cstdlib>
#include <string>
#include <cassert>
#include <cstdint>
#include <limits>

using namespace std::string_literals;

static std::string
dec_to_bin (int32_t number)
{
  std::string res;
  uint32_t num = static_cast<uint32_t> (number);
  while (num > 0)
    {
      res = ((num % 2 == 0) ? "0"s : "1"s) + res;
      num /= 2;
    }
  return res;
}

int
main ()
{
  {
    auto res = dec_to_bin (255);
    assert (res == "11111111");
  }
  {
    auto res = dec_to_bin (std::numeric_limits<int>::max ());
    assert (res == "1111111111111111111111111111111");
  }
  {
    auto res = dec_to_bin (-1);
    assert (res == "11111111111111111111111111111111");
  }
  {
    auto res = dec_to_bin (12378217);
    assert (res == "101111001110000001101001");
  }
  __builtin_printf ("All tests passed!\n");
  return EXIT_SUCCESS;
}
