#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstdint>

using u32 = std::uint32_t;
using i32 = std::int32_t;

class doubly_linked_list final
{
public:
  doubly_linked_list ()
    : _head {nullptr},
      _tail {nullptr},
      _size {0}
  {
  }

  ~doubly_linked_list ()
  {
    node* current {_head};

    while (current)
      {
        auto* n = current->_next;
        delete current;
        current = n;
      }
  }

  void
  append (i32 value)
  {
    if (empty ())
      {
        _head = _tail = new node (nullptr, nullptr, value);
      }
    else
      {
        _tail->_next = new node (_tail, nullptr, value);
        _tail = _tail->_next;
      }

    ++_size;
  }

  void
  prepend (i32 value)
  {
    if (empty ())
      {
        _head = _tail = new node (nullptr, nullptr, value);
      }
    else
      {
        _head->_prev = new node (nullptr, _head, value);
        _head = _head->_prev;
      }

    ++_size;
  }

  void
  remove (i32 value)
  {
    if (empty ())
      return;

    if (_head->_value == value)
      {
        auto* tmp = _head->_next;
        delete _head;

        if (tmp)
          {
            tmp->_prev = nullptr;
          }

        _head = tmp;
      }
    else if (_tail->_value == value)
      {
        auto* tmp = _tail->_prev;
        delete _tail;

        if (tmp)
          {
            tmp->_next = nullptr;
          }

        _tail = tmp;
      }
    else
      {
        node* current {_head->_next};

        while (current && current->_value != value)
          {
            current = current->_next;
          }

        if (!current)
          {
            return;
          }

        current->_prev->_next = current->_next;
        current->_next->_prev = current->_prev;

        delete current;
      }

    --_size;
  }

  bool
  contains (i32 value) const
  {
    if (empty ())
      {
        return false;
      }

    if (_head->_value == value)
      {
        return true;
      }
    else if (_tail->_value == value)
      {
        return true;
      }

    node* current {_head->_next};

    while (current && current->_value != value)
      current = current->_next;

    return current != nullptr;
  }

  void
  display () const
  {
    node* current {_head};

    while (current)
      {
        std::cout << current->_value << ' ';
        current = current->_next;
      }

    std::cout << '\n';
  }

  u32
  size () const
  {
    return _size;
  }

  bool
  empty () const
  {
    return size () == 0;
  }

private:
  struct node final
  {
    node* _prev;
    node* _next;
    i32 _value;
  };

  node* _head;
  node* _tail;
  u32   _size;
};

int
main ()
{
  doubly_linked_list list;

  // Test appending elements
  list.append (1);
  list.append (2);
  list.append (3);
  assert (list.size () == 3);
  assert (list.contains (1));
  assert (list.contains (2));
  assert (list.contains (3));

  // Test prepending elements
  list.prepend (0);
  assert (list.size () == 4);
  assert (list.contains (0));

  // Test removing elements
  list.remove (0);
  assert (list.size () == 3);
  assert (!list.contains (0));

  list.remove (2);
  assert (list.size () == 2);
  assert (! list.contains (2));

  list.remove (3);
  assert (list.size () == 1);
  assert (! list.contains (3));

  list.remove (1);
  assert (list.size () == 0);
  assert (! list.contains (1));
  assert (list.empty ());

  // Test removing from an empty list
  list.remove (1); // Should not crash
  assert (list.size () == 0);

  list.append (1);
  list.append (2);
  list.append (3);
  list.append (4);		// ensure destructor releases resources

  list.display ();

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
