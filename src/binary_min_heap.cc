#include <vector>
#include <cassert>
#include <cstdint>
#include <limits>
#include <iostream>
#include <optional>

class binary_min_heap final
{
public:
  binary_min_heap (uint32_t capacity)
    : _size  {0},
      _index {0}
  {
    if (capacity == 0)
      {
        throw std::runtime_error ("Capacity is 0.");
      }

    _data.resize (capacity + 1);
  }

  ~binary_min_heap () = default;

  void
  insert (int32_t value)
  {
    auto const cap = _data.capacity ();

    if (_size + 1 == cap)
      {
        _data.resize (cap * 2);
      }

    uint32_t hole = ++_index;

    _data[0] = value;

    for ( ; _data[0] < _data[hole / 2]; hole /= 2)
      {
        _data[hole] = _data[hole / 2];
      }

    _data[hole] = _data[0];

    ++_size;
  }

  void
  delete_min ()
  {
    if (empty ())
      {
        throw std::runtime_error ("Heap is empty.");
      }

    _data[1] = _data[_index--];

    percolate_down (1);

    --_size;
  }

  std::optional<int32_t>
  get_min () const
  {
    if (empty ())
      {
        return std::nullopt;
      }

    return _data[1];
  }

  bool
  empty () const
  {
    return size () == 0;
  }

  uint32_t
  size () const
  {
    return _size;
  }

private:
  void
  percolate_down (uint32_t hole)
  {
    int32_t hole_value = _data[hole];
    uint32_t child;

    for ( ; hole * 2 <= _index; hole *= 2)
      {
        child = hole * 2;

        if (child != _index && _data[child + 1] < _data[child])
          {
            ++child;
          }

        if (_data[child] < _data[hole])
          {
            _data[hole] = _data[child];
          }
        else
          {
            break;
          }
      }

    _data[hole] = hole_value;
  }

  std::vector<int32_t> _data;
  uint32_t _size;
  uint32_t _index;
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
