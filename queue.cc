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
  queue (u32 capacity = 8)
    : _data  {nullptr},
      _capacity {0},
      _size  {0},
      _front {0},
      _rear  {std::numeric_limits<u32>::max ()}
  {
    assert (capacity != 0);
    _capacity = capacity;
    _data = new i32[_capacity];
  }

  ~queue ()
  {
    delete[] _data;
  }

  void
  enqueue (int value)
  {
    if (_capacity == _size)
      throw std::runtime_error ("The queue is full");

    _rear = (_rear + 1) % _capacity;

    _data[_rear] = value;

    ++_size;
  }

  void
  dequeue ()
  {
    if (empty ())
      return;

    _front = (_front + 1) % _capacity;

    --_size;
  }

  i32
  front () const
  {
    if (empty ())
      throw std::runtime_error ("Queue is empty, cannot get front element");

    return _data[_front];
  }

  i32
  rear () const
  {
    if (empty ())
      throw std::runtime_error ("Queue is empty, cannot get rear element");

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
  u32  _capacity;
  u32  _size;
  u32  _front;
  u32  _rear;
};

int
main ()
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
  q.dequeue();

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

  // Test 7: Edge Case with Single Element
  queue q2 (1);
  q2.enqueue (10);
  assert (q2.front () == 10);
  assert (q2.rear () == 10);
  q2.dequeue ();
  assert (q2.empty ());

  // Test 8: Large Number of Operations
  queue q3 (1000);

  for (int i = 0; i < 1000; ++i)
    q3.enqueue (i);

  for (int i = 0; i < 1000; ++i)
    q3.dequeue ();

  assert (q3.empty ());

  std::cout << "Test passed!\n";

  return 0;
}
