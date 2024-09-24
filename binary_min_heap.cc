#include <vector>
#include <cassert>
#include <expected>
#include <climits>

enum class binary_min_heap_codes {
  empty
};

class binary_min_heap
{
public:
  binary_min_heap (int capacity)
    : _last_element_index {0}
  {
    assert (capacity > 0);

    _capacity = capacity;

    _data.resize (_capacity);
  }

  void
  insert (int value)
  {
    if (_last_element_index == _capacity - 1)
      {
	_capacity *= 2;
	_data.resize (_capacity);
      }

    int hole {++_last_element_index};

    _data[0] = std::move (value);

    for ( ; _data[0] < _data[hole / 2]; hole /= 2)
      _data[hole] = std::move (_data[hole / 2]);

    _data[hole] = std::move (_data[0]);
  }

  int
  get_min () const
  {
    if (empty ())
      return INT_MIN;

    return _data[1];
  }

  void
  delete_min ()
  {
    if (empty ())
      return;

    _data[1] = _data[_last_element_index--];

    percolate_down (1);
  }

  bool
  empty () const
  {
    return _last_element_index == 0;
  }

private:
  void
  percolate_down (int hole)
  {
    int hole_value = std::move (_data[hole]);
    int child;

    for ( ; hole * 2 <= _last_element_index; hole = child)
      {
	child = hole * 2;

	if (child != _last_element_index && _data[child + 1] < _data[child])
	  ++child;

	if (_data[child] < hole_value)
	  _data[hole] = std::move (_data[child]);
	else
	  break;
      }

    _data[hole] = std::move (hole_value);
  }

  std::vector<int> _data;
  int _last_element_index;
  int _capacity;
};

// class binary_min_heap final
// {
// public:
//   binary_min_heap (int capacity)
//     : _current_size {0}
//   {
//     assert (capacity > 0);

//     _data.resize (capacity + 1);
//   }

//   void
//   insert (int value)
//   {
//     if (_current_size == _data.capacity () - 1)
//       _data.resize (_current_size * 2);

//     // Create the hole.
//     int hole = ++_current_size;

//     _data[0] = std::move (value);

//     for ( ; value < _data[hole / 2]; hole /= 2)
//       // Move parent downward, and hole upward after.
//       _data[hole] = std::move (_data[hole / 2]);

//     // When the loop is finished, you are in the right position to insert.
//     _data[hole] = std::move (_data[0]);
//   }

//   std::expected<int, binary_min_heap_codes>
//   get_min () const
//   {
//     if (empty ())
//       return std::unexpected (binary_min_heap_codes::empty);

//     return _data[1];
//   }

//   void
//   delete_min ()
//   {
//     if (empty ())
//       return;

//     // Position X on top of the tree.
//     _data[1] = _data[_current_size--];

//     percolate_down (1);
//   }

//   bool
//   empty () const
//   {
//     return _current_size == 0;
//   }

// private:
//   void
//   percolate_down (int hole)
//   {
//     int child;
//     int hole_value = std::move (_data[hole]);

//     for ( ; hole * 2 <= _current_size; hole = child)
//       {
// 	child = hole * 2;

// 	if (child != _current_size && _data[child + 1] < _data[child])
// 	  ++child;

// 	if (_data[child] < hole_value)
// 	  _data[hole] = std::move (_data[child]);
// 	else
// 	  break;
//       }

//     _data[hole] = std::move (hole_value);
//   }

//   std::vector<int> _data;
//   int _current_size;
// };

int
main ()
{
  // Test 1: Basic insertion and get_min
  binary_min_heap h1(10);
  h1.insert(5);
  h1.insert(3);
  h1.insert(8);
  h1.insert(1);
  h1.insert(9);
  assert(h1.get_min() == 1);

  // Test 2: Deletion of minimum
  h1.delete_min();
  assert(h1.get_min() == 3);
  h1.delete_min();
  assert(h1.get_min() == 5);

  // Test 3: Empty heap
  binary_min_heap h2 (10);
  //assert(h2.get_min().error() == binary_min_heap_codes::empty);

  // Test 4: Large number of insertions
  binary_min_heap h3 (10);
  for (int i = 1000; i > 0; --i)
    h3.insert(i);

  assert(h3.get_min() == 1);

  // Test 5: Duplicate elements
  binary_min_heap h4 (10);
  h4.insert(5);
  h4.insert(3);
  h4.insert(3);
  h4.insert(1);
  h4.insert(1);
  assert(h4.get_min() == 1);
  h4.delete_min();
  assert(h4.get_min() == 1);

  // Test 6: Single element
  binary_min_heap h5 (10);
  h5.insert(42);
  assert(h5.get_min() == 42);
  h5.delete_min();
  assert(h5.empty());
}
