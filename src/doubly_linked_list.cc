#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

class doubly_linked_list final
{
public:
  doubly_linked_list () : _head{nullptr}, _tail{nullptr}, _size{0} {}

  ~doubly_linked_list ()
  {
    auto *node{_head};

    while (node)
      {
        auto *next = node->_next;
        delete node;
        node = next;
      }
  }

  void append (std::string const &value)
  {
    if (value.empty ())
      {
        return;
      }

    if (empty ())
      {
        _head = _tail = new node (_tail, nullptr, value);
      }
    else
      {
        _tail->_next = new node (_tail, nullptr, value);
        _tail = _tail->_next;
      }

    ++_size;
  }

  void prepend (std::string const &value)
  {
    if (value.empty ())
      {
        return;
      }

    if (empty ())
      {
        _head = _tail = new node (nullptr, _head, value);
      }
    else
      {
        _head->_prev = new node (nullptr, _head, value);
        _head = _head->_prev;
      }

    ++_size;
  }

  void remove (std::string const &value)
  {
    if (value.empty () || empty ())
      {
        return;
      }

    if (_head->_value == value)
      {
        auto *tmp = _head->_next;
        delete _head;
        _head = tmp;
        if (tmp)
          {
            _head->_prev = nullptr;
          }
      }
    else if (_tail->_value == value)
      {
        auto *tmp = _tail->_prev;
        delete _tail;
        _tail = tmp;
        if (tmp)
          {
            tmp->_next = nullptr;
          }
      }
    else
      {
        auto *node = _head->_next;

        while (node && node->_value != value)
          {
            node = node->_next;
          }

        if (!node)
          {
            return;
          }

        node->_prev->_next = node->_next;
        node->_next->_prev = node->_prev;
        delete node;
      }

    --_size;
  }

  bool contains (std::string const &value) const
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

    auto *node = _head->_next;

    while (node)
      {
        if (node->_value == value)
          {
            return true;
          }

        node = node->_next;
      }

    return false;
  }

  void display () const
  {
    auto *node = _head;

    while (node)
      {
        std::cout << node->_value << ' ';
        node = node->_next;
      }

    std::cout << '\n';
  }

  size_t size () const { return _size; }

  bool empty () const { return size () == 0; }

private:
  struct node final
  {
    node (node *prev, node *next, std::string value) : _prev{prev}, _next{next}, _value{value} {}

    node *_prev;
    node *_next;
    std::string _value;
  };

  node *_head;
  node *_tail;
  size_t _size;
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

  list.display ();

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
