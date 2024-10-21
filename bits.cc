#include <cassert>
#include <iostream>
#include <cstdint>

using i32 = std::int32_t;
using u32 = std::uint32_t;

u32
clear_bit (i32 n, u32 bitn)
{
  i32 mask = ~(1 << bitn);
  return n & mask;
}

u32
set_bit (i32 n, u32 bitn)
{
  return n | (1 << bitn);
}

u32
get_bit (i32 n, u32 bitn)
{
  return (n & (1 << bitn)) != 0;
}

u32
clear_msb_bits_through_bit (i32 n, u32 bitn)
{
  i32 mask = ~(-1 << bitn);
  return n & mask;
}

u32
clear_msb_bits_through_bit_second_version (i32 n, u32 bitn)
{
  return n & ((1 << bitn) - 1);
}

u32
clear_all_bits_through_end (i32 n, u32 bitn)
{
  i32 mask = (-1 << (bitn + 1));
  return n & mask;
}

u32
update_bit (i32 n, u32 bitn, u32 bit_value)
{
  bit_value = (bit_value == 0) ? 0 : 1;
  i32 mask = ~(1 << bitn);
  return (n & mask) | (bit_value << bitn);
}

int
main ()
{
  i32 number {0b1010};

  assert (clear_bit (number, 3) == 0b0010);
  assert (set_bit (number, 0)   == 0b1011);
  assert (get_bit (number, 3)   == 1);
  assert (clear_msb_bits_through_bit (number, 1) == 0b0);
  assert (clear_msb_bits_through_bit_second_version (number, 1) == 0b0);

  assert (clear_all_bits_through_end (0b01, 0) == 0b0);

  assert (update_bit (0b1111, 1, 0) == 0b1101);

  std::cout << "Test passed!\n";

  return EXIT_SUCCESS;
}
