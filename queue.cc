#include <cassert>
#include <new>
#include <list>
#include <limits>
#include <expected>

enum class queue_codes {
  overflow,
  underflow
};

class queue
{
public:
  queue (int capacity)
    : _data          {nullptr},
      _front         {0},
      _last_index    {std::numeric_limits<unsigned int>::max()},
      _size          {0}
  {
    assert (capacity > 0);

    _capacity = capacity;

    _data = new int[_capacity];
  }

  ~queue ()
  {
    delete[] _data;
  }

  std::expected<void, queue_codes>
  enqueue (int value)
  {
    if (_size == _capacity)
      return std::unexpected (queue_codes::overflow);

    _last_index = (_last_index + 1) % _capacity;

    _data[_last_index] = value;

    ++_size;

    return {};
  }

  std::expected<void, queue_codes>
  dequeue ()
  {
    if (empty ())
      return std::unexpected (queue_codes::underflow);

    _front = (_front + 1) % _capacity;

    --_size;

    return {};
  }

  std::expected<int, queue_codes>
  front ()
  {
    if (empty ())
      return std::unexpected (queue_codes::underflow);

    return _data[_front];
  }

  std::expected<int, queue_codes>
  rear ()
  {
    if (empty ())
      return std::unexpected (queue_codes::underflow);

    return _data[_last_index];
  }

  bool
  empty () const
  {
    return _size == 0;
  }

  unsigned int
  size () const
  {
    return _size;
  }

private:
  int* _data;
  int _capacity;
  unsigned int _front;
  unsigned int _last_index;
  unsigned int _size;
};


int
main ()
{
  queue q(3);

  // Test 1: Basic Operations
  assert (q.enqueue (1));
  assert (q.enqueue (2));
  assert (q.enqueue (3));
  assert (q.front ().value () == 1);
  assert (q.rear ().value () == 3);

  // Test 2: Overflow Condition
  auto overflow_result = q.enqueue (4);
  assert (overflow_result.error() == queue_codes::overflow);

  // Test 3: Underflow Condition
  assert (q.dequeue ());
  assert (q.dequeue ());
  assert (q.dequeue ());
  auto underflow_result = q.dequeue();
  assert (underflow_result.error () == queue_codes::underflow);

  // Test 4: Wrap-Around Behaviour
  assert (q.enqueue (4));
  assert (q.enqueue (5));
  assert (q.enqueue (6));
  assert (q.dequeue ());
  assert (q.enqueue (7));
  assert (q.front ().value () == 5);
  assert (q.rear ().value () == 7);

  // Test 5: Check Size Consistency
  assert (q.size () == 3);
  assert (q.dequeue ());
  assert (q.size () == 2);

  // Test 6: Front and Rear Consistency
  assert (q.front ().value () == 6);
  assert (q.rear ().value () == 7);

  // Test 7: Edge Case with Single Element
  queue q2 (1);
  assert (q2.enqueue (10));
  assert (q2.front ().value () == 10);
  assert (q2.rear ().value () == 10);
  assert (q2.dequeue ());
  assert (q2.empty ());

  // Test 8: Large Number of Operations
  queue q3 (1000);

  for (int i = 0; i < 1000; ++i)
    assert (q3.enqueue (i));

  for (int i = 0; i < 1000; ++i)
    assert (q3.dequeue ());

  assert (q3.empty ());

  return 0;
}
