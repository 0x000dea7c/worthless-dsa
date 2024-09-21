#include <vector>
#include <cassert>
#include <expected>

enum class binary_min_heap_codes {
  empty
};

class binary_min_heap final
{
public:
  binary_min_heap (int capacity)
    : _current_size {0}
  {
    assert (capacity > 0);

    _data.resize (capacity);
  }

  void
  insert (int value)
  {
    if (_current_size == _data.capacity () - 1)
      _data.resize (_current_size * 2);

    // Create the hole.
    int hole = ++_current_size;

    _data[0] = value;

    for ( ; value < _data[hole / 2]; hole /= 2)
      // Move parent downward, and hole upward after.
      _data[hole] = _data[hole / 2];

    // When the loop is finished, you are in the right position to insert.
    _data[hole] = _data[0];
  }

  std::expected<int, binary_min_heap_codes>
  get_min () const
  {
    if (empty ())
      return std::unexpected (binary_min_heap_codes::empty);

    return _data[1];
  }

  void
  delete_min ()
  {
    if (empty ())
      return;

    // Position X on top of the tree.
    _data[1] = _data[_current_size--];

    percolate_down (1);
  }

  bool
  empty () const
  {
    return _current_size == 0;
  }

private:
  void
  percolate_down (int hole)
  {
    int child;
    int hole_value {_data[hole]};

    for ( ; hole * 2 <= _current_size; hole = child)
      {
	child = hole * 2;

	if (child != _current_size && _data[child + 1] < _data[child])
	  ++child;

	if (_data[child] < _data[hole])
	  _data[hole] = _data[child];
	else
	  break;
      }

    _data[hole] = hole_value;
  }

  std::vector<int> _data;
  int _current_size;
};

int
main ()
{
  binary_min_heap h (100);

  assert (h.empty ());
  assert (h.get_min ().error () == binary_min_heap_codes::empty);

  h.insert (1);
  h.insert (4);
  h.insert (-1);

  assert (!h.empty ());
  assert (h.get_min () == -1);

  h.delete_min ();
  assert (h.get_min () == 1);

  h.delete_min ();
  assert (h.get_min () == 4);

  h.delete_min ();

  assert (h.empty ());

  return 0;
}
