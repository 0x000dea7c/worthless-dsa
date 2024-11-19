#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace std::string_literals;

class singly_linked_list final
{
public:
  singly_linked_list () : _head{nullptr}, _tail{nullptr}, _size{0} {}

  ~singly_linked_list ()
  {
    auto *node = _head;

    while (node)
      {
        auto *n = node->_next;
        delete node;
        node = n;
      }
  }

  void append (std::string const &value)
  {
    assert (!value.empty ());

    if (empty ())
      {
        _head = _tail = new node (nullptr, value);
      }
    else
      {
        _tail->_next = new node (nullptr, value);
        _tail = _tail->_next;
      }

    ++_size;
  }

  void prepend (std::string const &value)
  {
    assert (!value.empty ());

    if (empty ())
      {
        _head = _tail = new node (nullptr, value);
      }
    else
      {
        auto *p = new node (_head, value);
        _head = p;
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
    else if (_head->_value == value)
      {
        auto *n = _head->_next;
        delete _head;
        _head = n;
      }
    else
      {
        node *prev = nullptr;
        auto *node = _head;

        while (node && node->_value != value)
          {
            prev = node;
            node = node->_next;
          }

        if (!node)
          {
            return false;
          }

        if (node == _tail)
          {
            delete _tail;
            _tail = prev;
          }
        else
          {
            prev->_next = node->_next;
            delete node;
          }
      }

    --_size;

    return true;
  }

  bool contains (std::string const &value) const
  {
    auto *node = _head;

    while (node && node->_value != value)
      {
        node = node->_next;
      }

    return (node) ? true : false;
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
    node (node *next, std::string const &value) : _next{next}, _value{value} {}
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
  singly_linked_list list;
  list.append ("1"s);
  list.append ("2"s);
  list.append ("3"s);

  assert (list.size () == 3);
  assert (list.contains ("1"s));
  assert (list.contains ("2"s));
  assert (list.contains ("3"s));

  singly_linked_list list2;
  list2.prepend ("1"s);
  list2.prepend ("2"s);
  list2.prepend ("3"s);
  assert (list2.size () == 3);
  assert (list2.contains ("1"s));
  assert (list2.contains ("2"s));
  assert (list2.contains ("3"s));

  singly_linked_list list3;
  list3.append ("1"s);
  list3.append ("2"s);
  list3.append ("3"s);
  list3.remove ("2"s);
  assert (list3.size () == 2);
  assert (not list3.contains ("2"s));

  singly_linked_list list4;
  list4.append ("1"s);
  list4.append ("2"s);
  list4.remove ("3"s); // Element 3 does not exist
  assert (list4.size () == 2);

  singly_linked_list list5;
  list5.append ("5"s);
  list5.append ("10"s);
  assert (list5.contains ("5"s) == true);
  assert (list5.contains ("10"s) == true);
  assert (list5.contains ("15"s) == false);

  singly_linked_list list6;
  assert (list6.empty () == true);
  list6.remove ("1"s); // Should not crash
  assert (list6.size () == 0);

  singly_linked_list list7;
  list7.append ("1"s);
  list7.append ("2"s);
  list7.append ("3"s);
  list7.display (); // Expected output: 1 2 3

  singly_linked_list list8;
  list8.append ("1"s);
  list8.append ("2"s);
  list8.remove ("1"s);
  list8.remove ("2"s);
  assert (list8.empty () == true);

  singly_linked_list list9; // leak check
  list9.append ("1"s);
  list9.append ("2"s);
  list9.append ("3"s);

  singly_linked_list list10;
  list10.prepend ("5"s);
  list10.remove ("5"s);

  assert (list10.empty () == true);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
