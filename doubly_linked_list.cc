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
  {}

  ~doubly_linked_list ()
  {
    node* current {_head};

    while (current != nullptr)
      {
	auto* next = current->_next;
	delete current;
	current = next;
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
	auto* new_node = new node (_tail, nullptr, value);
	_tail->_next = new_node;
	_tail = new_node;
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
	auto* new_node = new node (nullptr, _head, value);
	_head->_prev = new_node;
	_head = new_node;
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
	auto* next_head  = _head->_next;
	if (next_head != nullptr)
	  next_head->_prev = nullptr;
	delete _head;
	_head = next_head;
      }
    else if (_tail->_value == value)
      {
	auto* prev_tail = _tail->_prev;
	if (prev_tail != nullptr)
	  prev_tail->_next = nullptr;
	delete _tail;
	_tail = prev_tail;
      }
    else
      {
	node* current {_head->_next};

	while (current != nullptr && current->_value != value)
	  current = current->_next;

	current->_prev->_next = current->_next;
	current->_next->_prev = current->_prev;
	delete current;
      }
    --_size;
  }

  bool
  contains (i32 value) const
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
    node (node* prev, node* next, i32 value)
      : _prev  {prev},
	_next  {next},
	_value {value}
    {
    }

    node* _prev;
    node* _next;
    i32 _value;
  };

  node* _head;
  node* _tail;
  u32   _size;
};

// class doubly_linked_list final
// {
// public:
//   doubly_linked_list ()
//     : _head {nullptr},
//       _tail {nullptr},
//       _size {0}
//   {}

//   ~doubly_linked_list ()
//   {
//     node* current {_head};

//     while (current != nullptr)
//       {
// 	auto* t = current->_next;
// 	delete current;
// 	current = t;
//       }
//   }

//   void
//   append (int value)
//   {
//     if (empty ())
//       {
// 	_head = _tail = new node (nullptr, nullptr, value);
//       }
//     else
//       {
// 	node* new_node = new node (_tail, nullptr, value);
// 	_tail->_next = new_node;
// 	_tail = new_node;
//       }

//     ++_size;
//   }

//   void
//   prepend (int value)
//   {
//     if (empty ())
//       {
// 	_head = _tail = new node (nullptr, nullptr, value);
//       }
//     else
//       {
// 	node* new_node = new node (nullptr, _head, value);
// 	_head->_prev = new_node;
// 	_head = new_node;
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
// 	auto* t = _head;
// 	_head = _head->_next;
// 	if (_head != nullptr)
// 	  _head->_prev = nullptr;
// 	delete t;
//       }
//     else if (_tail->_value == value)
//       {
// 	auto* t = _tail;
// 	_tail = _tail->_prev;
// 	if (_tail != nullptr)
// 	  _tail->_next = nullptr;
// 	delete t;
//       }
//     else
//       {
// 	node* current {_head->_next};

// 	while (current != nullptr && current->_value != value)
// 	  current = current->_next;

// 	if (current == nullptr)
// 	  return;

// 	current->_prev->_next = current->_next;
// 	current->_next->_prev = current->_prev;
// 	delete current;
//       }

//     --_size;
//   }

//   bool
//   contains (int value) const
//   {
//     node* current {_head};

//     while (current != nullptr && current->_value != value)
//       current = current->_next;

//     return current == nullptr ? false : true;
//   }

//   void
//   display () const
//   {
//     node* current {_head};

//     while (current != nullptr)
//       {
// 	std::cout << current->_value << ' ';
// 	current = current->_next;
//       }

//     std::cout << '\n';
//   }

//   u32
//   size () const
//   {
//     return _size;
//   }

//   bool
//   empty () const
//   {
//     return size () == 0;
//   }

// private:
//   struct node final
//   {
//     node (node* prev, node* next, int value)
//       : _prev  {prev},
// 	_next  {next},
// 	_value {value}
//     {}

//     node* _prev;
//     node* _next;
//     int _value;
//   };

//   node* _head;
//   node* _tail;
//   u32 _size;
// };

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
