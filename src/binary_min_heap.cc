#include <vector>
#include <cassert>
#include <iostream>
#include <optional>

class binary_min_heap final
{
public:
  binary_min_heap (size_t capacity)
    : _size  {0},
      _index {0}
  {
    if (capacity == 0)
      {
        throw std::runtime_error ("Capacity is 0");
      }

    _data.resize (capacity);
  }

  ~binary_min_heap () = default;

  void
  insert (int value)
  {
    if (_size + 1 == _data.capacity ())
      {
        _data.resize (_data.capacity () * 2);
      }

    _data[0] = std::move (value);

    size_t hole = ++_index;

    for ( ; _data[0] < _data[hole / 2]; hole /= 2)
      {
        _data[hole] = std::move (_data[hole / 2]);
      }

    _data[hole] = std::move (_data[0]);

    ++_size;
  }

  void
  delete_min ()
  {
    if (empty ())
      {
        return;
      }

    _data[1] = _data[_index--];

    percolate_down (1);

    --_size;
  }

  std::optional<int>
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

  size_t
  size () const
  {
    return _size;
  }

private:
  void
  percolate_down (size_t hole)
  {
    int hole_value = std::move (_data[hole]);
    size_t child;

    for ( ; hole * 2 <= _index; hole *= 2)
      {
        child = hole * 2;

        if (hole < _index && _data[child + 1] < _data[child])
          {
            ++child;
          }

        if (_data[child] < _data[hole])
          {
            _data[hole] = std::move (_data[child]);
          }
        else
          {
            break;
          }
      }

    _data[hole] = std::move (hole_value);
  }

  std::vector<int> _data;
  size_t _size;
  size_t _index;
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
