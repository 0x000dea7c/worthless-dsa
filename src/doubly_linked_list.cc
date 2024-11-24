#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

class doubly_linked_list
{
  struct node
  {
    node (node *prev, node *next, std::string const &val) : _prev{prev}, _next{next}, _val{val} {}
    node *_prev;
    node *_next;
    std::string _val;
  };

  node *_head;
  node *_tail;
  size_t _size;

public:
  doubly_linked_list () : _head{nullptr}, _tail{nullptr}, _size{0} {}

  ~doubly_linked_list ()
  {
    node *curr{_head};
    while (curr)
      {
        auto *n = curr->_next;
        delete curr;
        curr = n;
      }
  }

  void append (std::string const &value)
  {
    assert (!value.empty ());
    if (!_head)
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

  void prepend (std::string const &value)
  {
    assert (!value.empty ());
    if (!_tail)
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

  bool remove (std::string const &value)
  {
    assert (!value.empty ());
    if (empty ())
      {
        return false;
      }
    if (_head->_val == value)
      {
        auto *n = _head->_next;
        delete _head;
        if (n)
          {
            n->_prev = nullptr;
          }
        _head = n;
        --_size;
        return true;
      }
    else if (_tail->_val == value)
      {
        auto *n = _tail->_prev;
        delete _tail;
        if (n)
          {
            n->_next = nullptr;
          }
        _tail = n;
        --_size;
        return true;
      }
    auto *curr = _head->_next;
    while (curr && curr->_val != value)
      {
        curr = curr->_next;
      }
    if (!curr)
      {
        return false;
      }
    curr->_prev->_next = curr->_next;
    curr->_next->_prev = curr->_prev;
    delete curr;
    --_size;
    return true;
  }

  bool empty () const { return size () == 0; }

  size_t size () const { return _size; }

  bool contains (std::string const &value) const
  {
    if (empty ())
      {
        return false;
      }
    auto *curr = _head;
    while (curr && curr->_val != value)
      {
        curr = curr->_next;
      }
    return (curr) ? true : false;
  }
};

int
main ()
{
  using namespace std::string_literals;

  doubly_linked_list list;

  // Test appending elements
  list.append ("1"s);
  list.append ("2"s);
  list.append ("3"s);
  assert (list.size () == 3);
  assert (list.contains ("1"s));
  assert (list.contains ("2"s));
  assert (list.contains ("3"s));

  // Test prepending elements
  list.prepend ("0"s);
  assert (list.size () == 4);
  assert (list.contains ("0"s));

  // Test removing elements
  list.remove ("0"s);
  assert (list.size () == 3);
  assert (!list.contains ("0"s));

  list.remove ("2"s);
  assert (list.size () == 2);
  assert (!list.contains ("2"s));

  list.remove ("3"s);
  assert (list.size () == 1);
  assert (!list.contains ("3"s));

  list.remove ("1"s);
  assert (list.size () == 0);
  assert (!list.contains ("1"s));
  assert (list.empty ());

  // Test removing from an empty list
  list.remove ("1"s); // Should not crash
  assert (list.size () == 0);

  list.append ("1"s);
  list.append ("2"s);
  list.append ("3"s);
  list.append ("4"s); // ensure destructor releases resources

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
