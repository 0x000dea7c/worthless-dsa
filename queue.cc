#include <cassert>
#include <new>
#include <list>

class queue
{
public:
  queue ()
    : _data {}
  {
  }

  ~queue ()
  {
  }

  void
  enqueue (int value)
  {
    _data.emplace_back (value);
  }

  void
  dequeue ()
  {
    _data.pop_front ();
  }

  int
  front ()
  {
    return _data.front ();
  }

  int
  rear ()
  {
    return _data.back ();
  }

  bool
  empty () const
  {
    return _data.empty ();
  }

  unsigned int
  size () const
  {
    return _data.size ();
  }

private:
  std::list<int> _data;
};


int
main ()
{
  queue q;

  q.enqueue (1);
  q.enqueue (2);
  q.enqueue (3);

  assert (not q.empty ());
  assert (q.front () == 1);
  assert (q.rear ()  == 3);

  return 0;
}
