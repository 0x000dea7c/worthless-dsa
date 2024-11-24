#include <cassert>
#include <limits>
#include <string>
#include <cstdint>
#include <cstdlib>

using namespace std::string_literals;

static std::string
dec_to_hex (int32_t number)
{
  if (number == 0)
    {
      return "0"s;
    }
  std::string res;
  // convert to 2's complement for supporting neg input bc
  // the bit pattern for a neg num (in 2's compl) is the same as for the
  // positive version interpreted as unsigned
  uint32_t num = static_cast<uint32_t> (number);
  while (num > 0)
    {
      auto rem = num % 16;
      if (rem > 9)
        {
          res = char ('A' + (rem - 10)) + res;
        }
      else
        {
          res = char ('0' + rem) + res;
        }
      num /= 16;
    }
  return "0x" + res;
}

int
main ()
{
  {
    auto res = dec_to_hex (255);
    assert (res == "0xFF");
  }
  {
    auto res = dec_to_hex (std::numeric_limits<int>::max ());
    assert (res == "0x7FFFFFFF");
  }
  {
    auto res = dec_to_hex (-1);
    assert (res == "0xFFFFFFFF");
  }
  {
    auto res = dec_to_hex (12378217);
    assert (res == "0xBCE069");
  }
  __builtin_printf ("Tests passed!\n");
  return EXIT_SUCCESS;
}
