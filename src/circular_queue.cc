#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

class circular_queue
{
  size_t _front;
  size_t _back;
  size_t _size;
  std::vector<std::string> _data;

public:
  circular_queue (size_t capacity) : _front{0}, _back{0}, _size{0}
  {
    assert (capacity > 0);
    _data.resize (capacity);
  }

  ~circular_queue () = default;

  bool enqueue (std::string const &value)
  {
    assert (!value.empty ());
    if (_size == _data.capacity ())
      return false;
    _data[_back] = value;
    _back = (_back + 1) % _data.capacity ();
    ++_size;
    return true;
  }

  bool dequeue ()
  {
    if (empty ())
      return false;
    _data[_front].clear ();
    _front = (_front + 1) % _data.capacity ();
    --_size;
    return true;
  }

  std::optional<std::string> front () const
  {
    if (empty ())
      return std::nullopt;
    return _data[_front];
  }

  std::optional<std::string> rear () const
  {
    if (empty ())
      return std::nullopt;
    return _data[(_back == 0) ? _data.capacity () - 1 : _back - 1];
  }

  size_t capacity () const { return _data.capacity (); }

  bool empty () const { return size () == 0; }

  size_t size () const { return _size; }
};

int
main ()
{
  using namespace std::string_literals;

  {
    // Basic functionality.
    circular_queue q (8);

    assert (q.enqueue ("cause"s));
    assert (q.enqueue ("im"s));
    assert (q.enqueue ("losing"s));
    assert (q.enqueue ("my"s));
    assert (q.enqueue ("mind"s));

    assert (q.front () == "cause"s);
    assert (q.rear () == "mind"s);
    assert (!q.empty ());
    assert (q.size () == 5);
  }

  {
    // Dequeuing elements.
    circular_queue q (5);

    assert (q.enqueue ("between"s));
    assert (q.enqueue ("angels"s));
    assert (q.enqueue ("and"s));
    assert (q.enqueue ("insects"s));
    assert (q.enqueue ("can't go on"s));

    assert (q.dequeue ());
    assert (q.dequeue ());
    assert (q.dequeue ());
    assert (q.dequeue ());
    assert (q.dequeue ());

    // No more elements, should return false.
    assert (!q.dequeue ());
    assert (q.empty ());
    assert (q.size () == 0);
  }

  {
    // Fill queue, empty it and refill.
    circular_queue q (4);

    assert (q.enqueue ("this string is long so it's not optimised"s));
    assert (q.enqueue ("wawawiwa"s));
    assert (q.enqueue ("would this blow up?"s));
    assert (q.enqueue ("no way, right?"s));

    assert (q.dequeue ());
    assert (q.dequeue ());
    assert (q.dequeue ());
    assert (q.dequeue ());

    assert (q.enqueue ("new beginning"s));
    assert (q.enqueue ("new life"s));
    assert (q.enqueue ("new emotions"s));
    assert (q.enqueue ("new relationships"s));

    assert (!q.empty ());
    assert (q.size () == 4);
    assert (q.front () == "new beginning"s);
    assert (q.rear () == "new relationships"s);
  }

  {
    // Try to add more elements than supported.
    circular_queue q (3);

    assert (q.enqueue ("dog"s));
    assert (q.enqueue ("cat"s));
    assert (q.enqueue ("bird"s));

    assert (!q.enqueue ("crocodile"s));
  }

  {
    // More complex case.
    circular_queue q (5);

    q.enqueue ("i am"s);
    q.enqueue ("pointless"s);
    q.enqueue ("sky"s);
    q.enqueue ("sun"s);
    q.enqueue ("moon"s);

    q.dequeue ();
    q.dequeue ();
    q.dequeue ();
    q.dequeue ();
    q.dequeue ();

    q.enqueue ("china"s);
    q.enqueue ("japan"s);
    q.enqueue ("germany"s);
    q.enqueue ("spain"s);
    q.enqueue ("uruguay"s);

    assert (q.front () == "china"s);
    q.dequeue ();

    assert (q.front () == "japan"s);
    q.dequeue ();

    assert (q.front () == "germany"s);
    q.dequeue ();

    assert (q.front () == "spain"s);
    q.dequeue ();

    assert (q.front () == "uruguay"s);
    q.dequeue ();

    assert (q.empty ());
  }

  {
    // Lots of elements test.
    circular_queue q (10'000);

    for (size_t i = 0; i < q.capacity (); ++i)
      {
        std::string base{"a" + std::to_string (i)};
        assert (q.enqueue (base));
      }

    assert (q.front () == "a0"s);
    assert (q.rear () == "a9999"s);
  }

  std::cout << "All test passed!\n";

  return EXIT_SUCCESS;
}
