#include <cassert>
#include <new>

class dynamic_array
{
public:
  dynamic_array (int capacity = 8)
    : _data     {nullptr},
      _capacity {capacity},
      _size     {0}
  {
    assert (_capacity > 0);

    _data = new int[_capacity];
  }

  ~dynamic_array ()
  {
    delete[] _data;
  }

  void
  insert (int value)
  {
    if (_size == _capacity)
      {
	_capacity *= 2;

	auto* t = new int[_capacity];

	for (int i = 0; i < _size; ++i)
	  t[i] = _data[i];

	delete[] _data;

	_data = t;
      }

    _data[_size] = value;

    ++_size;
  }

  void
  remove (int value)
  {
    int i = 0;

    for ( ; i < _size; ++i)
      if (_data[i] == value)
	break;

    if (i == _size)
      return;

    for (int j = i; j < _size - 1; ++j)
      _data[j] = _data[j + 1];

    --_size;
  }

  int
  operator[] (int index)
  {
    if (empty () || index < 0 || index >= _size)
      return -1;

    return _data[index];
  }

  int
  size () const
  {
    return _size;
  }

  bool
  empty () const
  {
    return _size == 0;
  }

  int
  capacity () const
  {
    return _capacity;
  }

  bool
  contains (int value) const
  {
    for (int i = 0; i < _size; ++i)
      if (value == _data[i])
	return true;

    return false;
  }

  void
  set (int index, int value)
  {
    if (empty () || index < 0 || index >= _size)
      return;

    _data[index] = value;
  }

private:

  int* _data;
  int _size;
  int _capacity;
};

// class dynamic_array
// {
// public:
//   dynamic_array (int capacity = 8)
//     : _size {0}
//   {
//     assert (capacity > 0);

//     _capacity = capacity;

//     _data = new (std::nothrow) int[_capacity];
//   }

//   ~dynamic_array ()
//   {
//     delete[] _data;
//   }

//   void
//   insert (int value)
//   {
//     if (_size == _capacity)
//       {
// 	auto* tmp = new (std::nothrow) int[_capacity * 2];

// 	if (!tmp)
// 	  return;

// 	for (int i = 0; i < _capacity; ++i)
// 	  tmp[i] = _data[i];

// 	delete[] _data;

// 	_data = tmp;

// 	_capacity *= 2;
//       }

//     _data[_size++] = value;
//   }

//   void
//   remove (int value)
//   {
//     int i = 0;
//     bool found = false;

//     for ( ; i < _size; ++i)
//       {
// 	if (_data[i] == value)
// 	  {
// 	    found = true;
// 	    break;
// 	  }
//       }

//     if (found)
//       {
// 	for (int j = i; j < _size - 1; ++j)
// 	  _data[j] = _data[j + 1];
// 	--_size;
//       }
//   }

//   bool
//   contains (int value)
//   {
//     for (int i = 0; i < _size; ++i)
//       if (_data[i] == value)
// 	return true;

//     return false;
//   }

//   bool
//   empty () const
//   {
//     return _size == 0;
//   }

//   int
//   operator[] (int index)
//   {
//     assert (index >= 0 && index < _size);

//     return _data[index];
//   }

//   int
//   size () const
//   {
//     return _size;
//   }

// private:
//   int* _data;
//   int  _capacity;
//   int  _size;
// };

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

  d.remove (8);
  assert (d[0] == 9);
  assert (d[1] == 10);

  d.remove (9);
  assert (d[0] == 10);

  d.remove (10);

  assert (d.empty ());

  d.remove (10);			// Doesn't crash, doesn't do anything.

  dynamic_array d2;
  d2.insert (5);
  d2.insert (10);

  assert (d2.contains (5)  == true);
  assert (d2.contains (10) == true);
  assert (d2.contains (15) == false);

  // Resizing.
  dynamic_array d3 (1);
  d.insert (1);
  d.insert (2);
  assert (d[0] == 1);
  assert (d[1] == 2);

  dynamic_array d4;

  for (int i = 0; i < 1000; ++i)
    d4.insert (i);

  assert(d4.size () == 1000);
  assert(d4[999] == 999);

  d4.set (999, 69);
  d4.set (998, 71);

  assert (d4[999] == 69);
  assert (d4[998] == 71);

  return 0;
}
