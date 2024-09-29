#include <new>
#include <cassert>
#include <iostream>

class linked_list
{
public:
  linked_list ()
    : _head {nullptr},
      _tail {nullptr},
      _size {0}
  {}

  ~linked_list ()
  {
    node* previous {nullptr};

    while (_head != nullptr)
      {
	previous = _head;
	_head = _head->_next;
	delete previous;
      }
  }

  void
  append (int value)
  {
    if (empty ())
      _head = _tail = new node (_head, value);
    else
      {
	_tail->_next = new node (nullptr, value);
	_tail = _tail->_next;
      }

    ++_size;
  }

  void
  remove (int value)
  {
    node* current   {_head};
    node* previous {nullptr};

    while (current != nullptr && current->_value != value)
      {
	previous = current;
	current  = current->_next;
      }

    if (current == nullptr)
      return;

    if (current == _head && current == _tail)
      {
	delete current;
	_head = _tail = nullptr;
      }
    else if (current == _head)
      {
	previous = _head;
	_head = _head->_next;
	delete previous;
      }
    else if (current == _tail)
      {
	delete _tail;
	_tail = previous;
      }
    else
      {
	previous->_next = current->_next;
	delete current;
      }

    --_size;
  }

  bool
  contains (int value)
  {
    node* current {_head};

    while (current != nullptr && current->_value != value)
      current = current->_next;

    return current != nullptr ? true : false;
  }

  void
  display ()
  {
    node* current {_head};

    while (current != nullptr)
      {
	std::cout << current->_value << ' ';
	current = current->_next;
      }
  }

  int
  size () const
  {
    return _size;
  }

  void
  prepend (int value)
  {
    if (empty ())
      _head = _tail = new node (_head, value);
    else
      {
	auto* t = new node (_head, value);
	_head = t;
      }

    ++_size;
  }

  bool
  empty () const
  {
    return _size == 0;
  }

private:
  struct node
  {
    node (node* next, int value)
      : _next  {next},
	_value {value}
    {}

    node* _next;
    int _value;
  };

  node* _head;
  node* _tail;
  int _size;
};

// class linked_list
// {
// public:
//   linked_list ()
//     : _head {nullptr},
//       _tail {nullptr},
//       _size {0}
//   {
//   }

//   ~linked_list ()
//   {
//     auto* c = _head;

//     while (c != nullptr)
//       {
// 	auto* t = c->_next;
// 	delete c;
// 	c = t;
//       }
//   }

//   void
//   append (int value)
//   {
//     auto* new_node = new (std::nothrow) node (nullptr, value);

//     if (empty ())
//       _head = _tail = new_node;
//     else
//       {
// 	_tail->_next = new_node;
// 	_tail = _tail->_next;
//       }

//     ++_size;
//   }

//   void
//   remove (int value)
//   {
//     if (empty ())
//       return;

//     if (_head->_value == value)
//       {
// 	auto* n = _head->_next;
// 	delete _head;
// 	_head = n;
// 	--_size;
// 	return;
//       }

//     auto* c = _head;

//     while (c->_next != nullptr && c->_next->_value != value)
//       c = c->_next;

//     if (c->_next != nullptr)
//       {
// 	auto* t = c->_next->_next;
// 	delete c->_next;
// 	c->_next = t;
// 	--_size;

// 	if (c->_next == nullptr)
// 	  _tail = nullptr;
//       }
//   }

//   bool
//   contains (int value)
//   {
//     auto* c = _head;

//     while (c != nullptr)
//       {
// 	if (c->_value == value)
// 	  return true;

// 	c = c->_next;
//       }

//     return false;
//   }

//   void
//   display () const
//   {
//     auto* c = _head;

//     while (c != nullptr)
//       {
// 	std::cout << c->_value << ' ';
// 	c = c->_next;
//       }

//     std::cout << std::endl;
//   }

//   int
//   size () const
//   {
//     return _size;
//   }

//   bool
//   empty () const
//   {
//     return size () == 0;
//   }

//   void
//   prepend (int value)
//   {
//     if (empty ())
//       {
// 	append (value);
// 	return;
//       }

//     _head = new (std::nothrow) node (_head, value);

//     ++_size;
//   }

// private:
//   struct node
//   {
//     node (node* next, int value)
//       : _next  {next},
// 	_value {value}
//     {}

//     node* _next;
//     int _value;
//   };

//   node* _head;
//   node* _tail;
//   int _size;
// };

int
main ()
{
  linked_list list;
  list.append (1);
  list.append (2);
  list.append (3);

  assert (list.size () == 3);
  assert (list.contains (1));
  assert (list.contains (2));
  assert (list.contains (3));

  linked_list list2;
  list2.prepend (1);
  list2.prepend (2);
  list2.prepend (3);
  assert (list2.size () == 3);
  assert (list2.contains (1));
  assert (list2.contains (2));
  assert (list2.contains (3));

  linked_list list3;
  list3.append (1);
  list3.append (2);
  list3.append (3);
  list3.remove (2);
  assert (list3.size () == 2);
  assert (not list3.contains (2));

  linked_list list4;
  list4.append (1);
  list4.append (2);
  list4.remove (3); // Element 3 does not exist
  assert (list4.size () == 2);

  linked_list list5;
  list5.append (5);
  list5.append (10);
  assert (list5.contains (5) == true);
  assert (list5.contains (10) == true);
  assert (list5.contains (15) == false);

  linked_list list6;
  assert (list6.empty () == true);
  list6.remove (1); // Should not crash
  assert (list6.size () == 0);

  linked_list list7;
  list7.append (1);
  list7.append (2);
  list7.append (3);
  list7.display (); // Expected output: 1 2 3

  linked_list list8;
  list8.append (1);
  list8.append (2);
  list8.remove (1);
  list8.remove (2);
  assert (list8.empty () == true);

  return 0;
}
