#include <cstdlib>
#include <memory>
#include <iostream>
#include <cassert>

class doubly_linked_list
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
    if (empty ())
      return;

    node* current {_head};

    while (current != nullptr)
      {
	auto* t = current->_next;
	delete current;
	current = t;
      }
  }

  void
  append (int value)
  {
    if (empty ())
      _head = _tail = new node (nullptr, nullptr, value);
    else
      {
	auto* n = new node (_tail, nullptr, value);
	_tail->_next = n;
	_tail = n;
      }

    ++_size;
  }

  void
  prepend (int value)
  {
    if (empty ())
      _head = _tail = new node (nullptr, nullptr, value);
    else
      {
	auto* n = new node (nullptr, _head, value);
	_head->_prev = n;
	_head = n;
      }

    ++_size;
  }

  void
  remove (int value)
  {
    if (empty ())
      return;

    if (_head->_value == value)
      {
	auto* t = _head;
	_head = _head->_next;
	if (_head != nullptr)
	  _head->_prev = nullptr;
	delete t;
      }
    else if (_tail->_value == value)
      {
	auto* t = _tail;
	_tail = _tail->_prev;
	if (_tail != nullptr)
	  _tail->_next = nullptr;
	delete t;
      }
    else
      {
	node* current  {_head->_next};

	while (current != nullptr && current->_value != value)
	  current  = current->_next;

	node* previous {current->_prev};
	node* next     {current->_next};

	previous->_next = next;
	next->_prev = previous;
	delete current;
      }

    --_size;
  }

  bool
  contains (int value) const
  {
    node* current {_head};

    while (current != nullptr && current->_value != value)
      current = current->_next;

    return (current == nullptr) ? false : true;
  }

  void
  display () const
  {
    if (empty ())
      return;

    node* current {_head};

    while (current != nullptr)
      {
	std::cout << current->_value << ' ';
	current = current->_next;
      }

    std::cout << '\n';
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

private:
  struct node final
  {
    node (node* prev, node* next, int value)
      : _prev  {prev},
	_next  {next},
	_value {value}
    {}

    node *_prev;
    node *_next;
    int _value;
  };

  node* _head;
  node* _tail;
  int   _size;
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
