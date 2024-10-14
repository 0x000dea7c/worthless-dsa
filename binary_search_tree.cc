#include <cassert>
#include <iostream>
#include <stack>
#include <cstdint>

using u32 = std::uint32_t;
using i32 = std::int32_t;

class binary_search_tree final
{
public:
  binary_search_tree ()
    : _root {nullptr}
  {
  }

  ~binary_search_tree ()
  {
    make_empty (_root);
  }

  void
  insert (i32 key, i32 value)
  {
    insert (_root, key, value);
  }

  void
  remove (i32 key)
  {
    remove (_root, key);
  }

  bool
  contains (i32 key) const
  {
    return contains (_root, key);
  }

  void
  preorder_print () const
  {
    preorder_print (_root);
  }

  void
  inorder_print () const
  {
    inorder_print (_root);
  }

  void
  postorder_print () const
  {
    postorder_print (_root);
  }

  bool
  empty () const
  {
    return _root == nullptr;
  }

private:
  struct node final
  {
    node (node* left, node* right, i32 key, i32 value)
      : _left  {left},
	_right {right},
	_key   {key},
	_value {value}
    {}

    node* _left;
    node* _right;
    i32   _key;
    i32   _value;
  };

  void
  insert (node*& current, i32 key, i32 value)
  {
    if (!current)
      current = new node (nullptr, nullptr, key, value);
    else if (key > current->_key)
      insert (current->_right, key, value);
    else if (key < current->_key)
      insert (current->_left, key, value);
    else
      ; // don't do shit cause we don't support dups
  }

  void
  remove (node*& current, i32 key)
  {
    if (current == nullptr)
      return;

    if (key > current->_key)
      remove (current->_right, key);
    else if (key < current->_key)
      remove (current->_left, key);
    else
      {
	if (current->_right && current->_left)
	  {
	    auto* min = find_min (current->_right);
	    current->_value = min->_value;
	    current->_key = min->_key;
	    remove (current->_right, current->_key);
	  }
	else
	  {
	    auto* child = current->_right ? current->_right : current->_left;
	    delete current;
	    current = child;
	  }
      }
  }

  void
  preorder_print (node* current) const
  {
    if (current)
      {
	std::cout << current->_key << ' ';
	preorder_print (current->_left);
	preorder_print (current->_right);
      }
  }

  void
  inorder_print (node* current) const
  {
    if (current)
      {
	inorder_print (current->_left);
	std::cout << current->_key << ' ';
	inorder_print (current->_right);
      }
  }

  void
  postorder_print (node* current) const
  {
    if (current)
      {
	postorder_print (current->_left);
	postorder_print (current->_right);
	std::cout << current->_key << ' ';
      }
  }

  void
  make_empty (node* current)
  {
    if (current)
      {
	make_empty (current->_left);
	make_empty (current->_right);
	delete current;
      }
  }

  bool
  contains (node* current, i32 key) const
  {
    if (!current)
      return false;

    if (key > current->_key)
      return contains (current->_right, key);
    else if (key < current->_key)
      return contains (current->_left, key);
    else
      return true;
  }

  node*
  find_min (node* current) const
  {
    while (current->_left)
      current = current->_left;
    return current;
  }

  node* _root;
};

int
main ()
{
  std::cout << "Testing binary search tree...";

  binary_search_tree tree;

  tree.insert (10, 10);
  tree.insert ( 5,  5);
  tree.insert (20, 20);

  tree.insert ( 3,  3);
  tree.insert ( 6,  6);

  tree.insert (30, 30);
  tree.insert (11, 11);

  assert (tree.contains (10) == true);
  assert (tree.contains (5) == true);
  assert (tree.contains (20) == true);

  assert (tree.contains (3) == true);
  assert (tree.contains (6) == true);

  assert (tree.contains (30) == true);
  assert (tree.contains (11) == true);

  assert (tree.contains (999) == false);
  assert (tree.contains (-20) == false);

  tree.remove (11);		// leaf
  tree.remove (30);		// leaf

  tree.remove (6);		// leaf
  tree.remove (5);		// parent

  assert (tree.contains (11) == false);
  assert (tree.contains (30) == false);
  assert (tree.contains (6)  == false);
  assert (tree.contains (5)  == false);
  assert (tree.contains (3)  == true);

  binary_search_tree tree2;

  for (int i = 0; i < 1000; ++i)
    tree2.insert (i, i);

  assert (! tree2.empty ());

  for (int i = 0; i < 1000; ++i)
    tree2.remove (i);

  assert (tree2.empty ());

  binary_search_tree complex_tree;

  // Insert a series of nodes
  complex_tree.insert(50, 50);
  complex_tree.insert(30, 30);
  complex_tree.insert(70, 70);
  complex_tree.insert(20, 20);
  complex_tree.insert(40, 40);
  complex_tree.insert(60, 60);
  complex_tree.insert(80, 80);

  // Check if all inserted nodes are present
  assert(complex_tree.contains(50) == true);
  assert(complex_tree.contains(30) == true);
  assert(complex_tree.contains(70) == true);
  assert(complex_tree.contains(20) == true);
  assert(complex_tree.contains(40) == true);
  assert(complex_tree.contains(60) == true);
  assert(complex_tree.contains(80) == true);

  // Remove a node with two children
  complex_tree.remove(30);
  assert(complex_tree.contains(30) == false);

  // Remove a leaf node
  complex_tree.remove(20);
  assert(complex_tree.contains(20) == false);

  // Remove a node with one child
  complex_tree.remove(70);
  assert(complex_tree.contains(70) == false);

  // Check remaining nodes
  assert(complex_tree.contains(50) == true);
  assert(complex_tree.contains(40) == true);
  assert(complex_tree.contains(60) == true);
  assert(complex_tree.contains(80) == true);

  // Insert more nodes
  complex_tree.insert(65, 65);
  complex_tree.insert(55, 55);
  complex_tree.insert(75, 75);

  // Check new insertions
  assert(complex_tree.contains(65) == true);
  assert(complex_tree.contains(55) == true);
  assert(complex_tree.contains(75) == true);

  // Remove root node
  complex_tree.remove(50);
  assert(complex_tree.contains(50) == false);

  // Check tree structure after complex operations
  assert(complex_tree.contains(40) == true);
  assert(complex_tree.contains(60) == true);
  assert(complex_tree.contains(80) == true);
  assert(complex_tree.contains(65) == true);
  assert(complex_tree.contains(55) == true);
  assert(complex_tree.contains(75) == true);

  std::cout << "\n...Printing preorder...\n";
  complex_tree.preorder_print ();

  std::cout << "\n...Printing inorder...\n";
  complex_tree.inorder_print ();

  std::cout << "\n...Printing postorder...\n";
  complex_tree.postorder_print ();

  // Final removals to empty the tree
  complex_tree.remove(40);
  complex_tree.remove(60);
  complex_tree.remove(80);
  complex_tree.remove(65);
  complex_tree.remove(55);
  complex_tree.remove(75);

  // Ensure the tree is empty
  assert(complex_tree.empty() == true);

  std::cout << "ok...\n";

  return 0;
}
