#include <vector>
#include <cassert>
#include <iostream>
#include <optional>

class binary_min_heap
{
  size_t _index;
  size_t _size;
  std::vector<int> _data;

  void percolate_down (size_t hole)
  {
    int hole_value = _data[hole];
    size_t child;
    for (; hole <= _index; hole *= 2)
      {
        child = hole * 2;
        if (child <= _index && _data[child + 1] < _data[child])
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

public:
  binary_min_heap (size_t capacity) : _index{0}, _size{0}
  {
    assert (capacity > 0);
    _data.resize (capacity + 1);
  }

  ~binary_min_heap () = default;

  void insert (int value)
  {
    auto const cap = _data.capacity ();
    if (_size + 1 == cap)
      {
        _data.resize (cap * 2);
      }
    _data[0] = value;
    size_t hole = ++_index;
    for (; _data[0] < _data[hole / 2]; hole /= 2)
      {
        _data[hole] = _data[hole / 2];
      }
    _data[hole] = _data[0];
    ++_size;
  }

  std::optional<int> get_min () const
  {
    if (empty ())
      {
        return std::nullopt;
      }
    return _data[1];
  }

  bool delete_min ()
  {
    if (empty ())
      {
        return false;
      }
    _data[1] = _data[_index--];
    percolate_down (1);
    --_size;
    return true;
  }

  bool empty () const { return size () == 0; }

  size_t size () const { return _size; }
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
    // Test 3: Large number of insertions, test resize
    binary_min_heap heap (10);

    for (int i = 1000; i > 0; --i)
      heap.insert (i);

    assert (heap.get_min () == 1);
  }

  {
    // Test 4: Duplicate elements (supported)
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
    // Test 5: Single element
    binary_min_heap heap (10);
    heap.insert (42);

    assert (heap.get_min () == 42);

    heap.delete_min ();

    assert (heap.empty ());
  }

  std::cout << "All tests passed!\n";
}
