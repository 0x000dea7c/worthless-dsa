#include <vector>
#include <cassert>
#include <cstdint>
#include <limits>
#include <iostream>

using i32 = std::int32_t;
using u32 = std::int32_t;

class binary_min_heap final
{
public:
  binary_min_heap (u32 capacity)
    : _capacity {capacity},
      _size     {0},
      _index    {0}
  {
    _data.resize (capacity);
  }

  ~binary_min_heap () = default;

  void
  insert (i32 value)
  {
    if (_size + 1 == _capacity)
      {
	_capacity *= 2;
	_data.resize (_capacity);
      }

    u32 hole = ++_index;

    _data[0] = value;

    for ( ; _data[0] < _data[hole / 2]; hole /= 2)
      _data[hole] = _data[hole / 2];

    _data[hole] = _data[0];

    ++_size;
  }

  i32
  get_min () const
  {
    if (empty ())
      return std::numeric_limits<i32>::min ();

    return _data[1];
  }

  void
  delete_min ()
  {
    if (empty ())
      return;

    _data[1] = _data[_index--];

    percolate_down (1);

    --_size;
  }

  bool
  empty () const
  {
    return _size == 0;
  }

private:
  void
  percolate_down (i32 hole)
  {
    i32 hole_value = _data[hole];
    u32 child;

    for ( ; hole <= _index; hole *= 2)
      {
	child = hole * 2;

	if (child != _index && _data[child + 1] < _data[child])
	  ++child;

	if (_data[child] < _data[hole])
	  _data[hole] = _data[child];
	else
	  break;
      }

    _data[hole] = hole_value;
  }

  std::vector<i32> _data;
  u32 _size;
  u32 _capacity;
  u32 _index;
};

int
main ()
{
  // Test 1: Basic insertion and get_min
  binary_min_heap h1 (10);
  h1.insert (5);
  h1.insert (3);
  h1.insert (8);
  h1.insert (1);
  h1.insert (9);
  assert (h1.get_min () == 1);

  // Test 2: Deletion of minimum
  h1.delete_min ();
  assert (h1.get_min() == 3);
  h1.delete_min ();
  assert (h1.get_min() == 5);

  // Test 3: Empty heap
  binary_min_heap h2 (10);

  // Test 4: Large number of insertions
  binary_min_heap h3 (10);
  for (int i = 1000; i > 0; --i)
    h3.insert (i);

  assert (h3.get_min () == 1);

  // Test 5: Duplicate elements
  binary_min_heap h4 (10);
  h4.insert (5);
  h4.insert (3);
  h4.insert (3);
  h4.insert (1);
  h4.insert (1);
  assert(h4.get_min () == 1);
  h4.delete_min ();
  assert(h4.get_min () == 1);

  // Test 6: Single element
  binary_min_heap h5 (10);
  h5.insert (42);
  assert (h5.get_min () == 42);
  h5.delete_min ();
  assert (h5.empty ());

  std::cout << "All tests passed!\n";
}
