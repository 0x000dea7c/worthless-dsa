#include <cassert>
#include <limits>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <iostream>

using u32 = std::uint32_t;
using i32 = std::int32_t;

class queue final
{
public:
  queue (u32 capacity)
    : _data     {nullptr},
      _front    {0},
      _rear     {std::numeric_limits<u32>::max ()},
      _capacity {capacity},
      _size     {0}
  {
    if (_capacity == 0)
      throw std::invalid_argument ("Queue's capacity cannot be 0.");

    _data = new i32 [_capacity];
  }

  ~queue ()
  {
    delete[] _data;
  }

  void
  enqueue (i32 value)
  {
    if (_size == _capacity)
      throw std::runtime_error ("The queue is full");

    _rear = (_rear + 1) % _capacity;

    _data[_rear] = value;

    ++_size;
  }

  void
  dequeue ()
  {
    if (empty ())
      throw std::runtime_error ("The queue is empty");

    _front = (_front + 1) % _capacity;

    --_size;
  }

  i32
  front () const
  {
    if (empty ())
      throw std::runtime_error ("The queue is empty");

    return _data[_front];
  }

  i32
  rear () const
  {
    if (empty ())
      throw std::runtime_error ("The queue is empty");

    return _data[_rear];
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
  i32* _data;
  u32 _front;
  u32 _rear;
  u32 _capacity;
  u32 _size;
};

int
main ()
{
  {
    queue q (3);

    // Test 1: Basic Operations
    q.enqueue (1);
    q.enqueue (2);
    q.enqueue (3);
    assert (q.front () == 1);
    assert (q.rear () == 3);

    // Test 2: Overflow Condition
    try
      {
	q.enqueue (4);
	assert (false && "can't get here, it needs to overflow");
      }
    catch (std::exception const& err)
      {
	assert (std::strcmp (err.what (), "The queue is full") == 0);
      }

    // Test 3: Underflow Condition
    q.dequeue ();
    q.dequeue ();
    q.dequeue ();

    try
      {
	q.dequeue ();
	assert (false && "you shouldn't get here");
      }
    catch (std::exception const& err)
      {
	assert (std::strcmp (err.what (), "The queue is empty") == 0);
      }

    // Test 4: Wrap-Around Behaviour
    q.enqueue (4);
    q.enqueue (5);
    q.enqueue (6);
    q.dequeue ();
    q.enqueue (7);
    assert (q.front () == 5);
    assert (q.rear () == 7);

    // Test 5: Check Size Consistency
    assert (q.size () == 3);
    q.dequeue ();
    assert (q.size () == 2);

    // Test 6: Front and Rear Consistency
    assert (q.front () == 6);
    assert (q.rear () == 7);
  }

  {
    // Test 7: Edge Case with Single Element
    queue q (1);
    q.enqueue (10);
    assert (q.front () == 10);
    assert (q.rear () == 10);
    q.dequeue ();
    assert (q.empty ());
  }

  {
    // Test 8: Large Number of Operations
    queue q (1000);

    for (int i = 0; i < 1000; ++i)
      q.enqueue (i);

    for (int i = 0; i < 1000; ++i)
      q.dequeue ();

    assert (q.empty ());
  }

  std::cout << "Test passed!\n";

  return 0;
}
