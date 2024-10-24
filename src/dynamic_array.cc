#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <iostream>

using u32 = std::uint32_t;
using i32 = std::int32_t;

class dynamic_array final
{
public:
  dynamic_array (u32 capacity = 8)
    : _data     {nullptr},
      _size     {0},
      _capacity {0}
  {
    assert (capacity != 0);
    _capacity = capacity;
    _data = new i32[_capacity];
  }

  ~dynamic_array ()
  {
    delete[] _data;
  }

  void
  insert (i32 value)
  {
    if (_size == _capacity)
      {
	_capacity *= 2;

	i32* new_data = new i32[_capacity];

	for (u32 i {}; i < _size; ++i)
	  new_data[i] = _data[i];

	delete[] _data;

	_data = new_data;
      }

    _data[_size++] = value;
  }

  void
  pop_back ()
  {
    if (empty ())
      return;

    --_size;
  }

  i32
  operator[] (u32 index) const
  {
    if (index > _size - 1)
      throw std::runtime_error ("Out of bounds");

    return _data[index];
  }

  u32
  size () const
  {
    return _size;
  }

  bool
  empty () const
  {
    return size () == 0;
  }

  u32
  capacity () const
  {
    return _capacity;
  }

  bool
  contains (i32 value) const
  {
    for (u32 i {}; i < _size; ++i)
      if (value == _data[i])
	return true;
    return false;
  }

  void
  set (u32 index, i32 value)
  {
    if (index > _size - 1)
      throw std::runtime_error ("Out of bounds");

    _data[index] = value;
  }

  void
  remove (u32 index)
  {
    if (index > _size - 1)
      throw std::runtime_error ("Out of bounds");

    if (empty ())
      return;

    for (u32 i {index}; i < _size; ++i)
      _data[i] =_data[i + 1];

    --_size;
  }

private:
  i32* _data;
  u32  _size;
  u32  _capacity;
};

int
main ()
{
  dynamic_array d;

  d.insert (8);
  d.insert (9);
  d.insert (10);

  assert (not d.empty ());
  assert (d[0] == 8);
  assert (d[1] == 9);
  assert (d[2] == 10);

  d.remove (0);
  assert (d[0] == 9);
  assert (d[1] == 10);

  d.remove (0);
  assert (d[0] == 10);

  d.remove (0);

  assert (d.empty ());

  try
    {
      d.remove (10);
    }
  catch (std::runtime_error const& e)
    {
      assert (std::strcmp (e.what (), "Out of bounds") == 0);
    }

  d.insert (1);
  d.insert (2);

  assert (d[0] == 1);
  assert (d[1] == 2);

  dynamic_array d2;
  d2.insert (5);
  d2.insert (10);

  assert (d2.contains (5)  == true);
  assert (d2.contains (10) == true);
  assert (d2.contains (15) == false);

  // Resizing.
  dynamic_array d3 (1);
  d3.insert (1);
  d3.insert (2);
  assert (d3[0] == 1);
  assert (d3[1] == 2);

  dynamic_array d4;

  for (int i = 0; i < 1000; ++i)
    d4.insert (i);

  assert(d4.size () == 1000);
  assert(d4[999] == 999);

  d4.set (999, 69);
  d4.set (998, 71);

  assert (d4[999] == 69);
  assert (d4[998] == 71);

  dynamic_array d5;
  d5.insert (1);
  d5.insert (2);
  d5.insert (3);
  d5.insert (4);
  d5.insert (5);

  d5.pop_back ();
  assert (d5[3] == 4);
  assert (d5[2] == 3);
  assert (d5[1] == 2);
  assert (d5[0] == 1);

  d4.pop_back ();
  assert (d5[2] == 3);
  assert (d5[1] == 2);
  assert (d5[0] == 1);

  d3.pop_back ();
  assert (d5[1] == 2);
  assert (d5[0] == 1);

  d3.pop_back ();
  assert (d5[0] == 1);

  d3.pop_back ();

  assert (d3.empty ());

  std::cout << "Test passed!\n";

  return 0;
}
