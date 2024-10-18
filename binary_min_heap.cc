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
    : _size  {0},
      _index {0}
  {
    if (capacity == 0)
      throw std::runtime_error ("capacity is 0");

    _data.resize (capacity);
  }

  ~binary_min_heap () = default;

  void
  insert (i32 value)
  {
    if (_size + 1 == _data.capacity ())
      {
	auto cap = _data.capacity ();
	_data.resize (cap * 2);
      }

    _data[0] = value;

    u32 hole = ++_index;

    for ( ; _data[0] < _data[hole / 2]; hole /= 2)
      _data[hole] = _data[hole / 2];

    _data[hole] = _data[0];

    ++_size;
  }

  i32
  get_min () const
  {
    if (empty ())
      throw std::runtime_error ("heap is empty");

    return _data[1];
  }

  void
  delete_min ()
  {
    if (empty ())
      throw std::runtime_error ("heap is empty");

    _data[1] = _data[_index--];

    percolate_down (1);

    --_size;
  }

  bool
  empty () const
  {
    return size () == 0;
  }

  u32
  size () const
  {
    return _size;
  }

private:
  void
  percolate_down (u32 hole)
  {
    i32 hole_value = _data[hole];
    i32 child;

    for ( ; hole * 2 <= _index; hole *= 2)
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
  u32 _index;
};

int
main ()
{
  {
    // Test 1: Basic insertion and get_min
    binary_min_heap heap (10);
    heap.insert (5);
    heap.insert (3);
    heap.insert (8);
    heap.insert (1);
    heap.insert (9);
    assert (heap.get_min () == 1);
  }

  {
    // Test 2: Insertion and deletion
    binary_min_heap heap (10);
    heap.insert (5);
    heap.insert (9);
    heap.insert (1);

    assert (heap.get_min () == 1);

    heap.delete_min ();

    assert (heap.get_min () == 5);

    heap.delete_min ();

    assert (heap.get_min () == 9);
  }

  {
    // Test 3: empty and exception is thrown
    try
      {
	binary_min_heap heap (0);
	assert (false && "can't get here cuz exception will be thrown");
      }
    catch (std::runtime_error const&)
      {
      }
  }

  {
    // Test 4: Large number of insertions, test resize
    binary_min_heap heap (10);

    for (int i = 1000; i > 0; --i)
      heap.insert (i);

    assert (heap.get_min () == 1);
  }

  {
    // Test 5: Duplicate elements (supported)
    binary_min_heap heap (10);
    heap.insert (5);
    heap.insert (3);
    heap.insert (3);
    heap.insert (1);
    heap.insert (1);

    assert (heap.get_min () == 1);

    heap.delete_min ();

    assert (heap.get_min () == 1);
  }

  {
    // Test 6: Single element
    binary_min_heap heap (10);
    heap.insert (42);

    assert (heap.get_min () == 42);

    heap.delete_min ();

    assert (heap.empty ());
  }

  std::cout << "All tests passed!\n";
}
