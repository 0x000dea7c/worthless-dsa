#include <cassert>
#include <iostream>
#include <cstdlib>

class singly_linked_list final
{
public:
  singly_linked_list ()
    : _head {nullptr},
      _tail {nullptr},
      _size {0}
  {}

  ~singly_linked_list ()
  {
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
      _head = _tail = new node (nullptr, value);
    else
      {
	auto* n = new node (nullptr, value);
	_tail->_next = n;
	_tail = _tail->_next;
      }
    ++_size;
  }

  void
  prepend (int value)
  {
    if (empty ())
      _head = _tail = new node (nullptr, value);
    else
      {
	auto* n = new node (_head, value);
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
	if (_head == _tail)
	  {
	    delete _head;
	    _head = _tail = nullptr;
	  }
	else
	  {
	    auto* t = _head;
	    _head = _head->_next;
	    delete t;
	  }
      }
    else if (_tail->_value == value)
      {
	node* current  {_head};

	while (current->_next != _tail)
	  current = current->_next;

	current->_next = nullptr;
	delete _tail;
	_tail = current;
      }
    else
      {
	node* previous {nullptr};
	node* current  {_head};

	while (current != nullptr && current->_value != value)
	  {
	    previous = current;
	    current  = current->_next;
	  }

	if (current == nullptr)
	  return;

	previous->_next = current->_next;
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

    return current != nullptr ? true : false;
  }

  void
  display () const
  {
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
    return size () == 0;
  }

private:
  struct node final
  {
    node* _next;
    int _value;
  };

  node* _head;
  node* _tail;
  int _size;
};

int
main ()
{
  singly_linked_list list;
  list.append (1);
  list.append (2);
  list.append (3);

  assert (list.size () == 3);
  assert (list.contains (1));
  assert (list.contains (2));
  assert (list.contains (3));

  singly_linked_list list2;
  list2.prepend (1);
  list2.prepend (2);
  list2.prepend (3);
  assert (list2.size () == 3);
  assert (list2.contains (1));
  assert (list2.contains (2));
  assert (list2.contains (3));

  singly_linked_list list3;
  list3.append (1);
  list3.append (2);
  list3.append (3);
  list3.remove (2);
  assert (list3.size () == 2);
  assert (not list3.contains (2));

  singly_linked_list list4;
  list4.append (1);
  list4.append (2);
  list4.remove (3); // Element 3 does not exist
  assert (list4.size () == 2);

  singly_linked_list list5;
  list5.append (5);
  list5.append (10);
  assert (list5.contains (5) == true);
  assert (list5.contains (10) == true);
  assert (list5.contains (15) == false);

  singly_linked_list list6;
  assert (list6.empty () == true);
  list6.remove (1); // Should not crash
  assert (list6.size () == 0);

  singly_linked_list list7;
  list7.append (1);
  list7.append (2);
  list7.append (3);
  list7.display (); // Expected output: 1 2 3

  singly_linked_list list8;
  list8.append (1);
  list8.append (2);
  list8.remove (1);
  list8.remove (2);
  assert (list8.empty () == true);

  singly_linked_list list9;	// leak check
  list9.append (1);
  list9.append (2);
  list9.append (3);

  singly_linked_list list10;
  list10.prepend (5);
  list10.remove (5);

  assert (list10.empty () == true);

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
