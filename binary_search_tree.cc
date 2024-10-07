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
  {

  }

  ~binary_search_tree ()
  {
    make_empty (_root);
  }

  void
  insert (i32 key, i32 value)
  {
    insert (key, value, _root);
  }

  void
  remove (i32 key)
  {
    remove (key, _root);
  }

  bool
  contains (i32 key) const
  {
    return contains (key, _root);
  }

  bool
  empty () const
  {
    return _root == nullptr;
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
    i32 _key;
    i32 _value;
  };

  void
  make_empty (node* current)
  {
    if (current != nullptr)
      {
	make_empty (current->_left);
	make_empty (current->_right);
	delete current;
      }
  }

  void
  insert (i32 key, i32 value, node*& current)
  {
    if (current == nullptr)
      current = new node (nullptr, nullptr, key, value);
    else if (key > current->_key)
      insert (key, value, current->_right);
    else if (key < current->_key)
      insert (key, value, current->_left);
    else
      return;			// no duplicates
  }

  void
  remove (i32 key, node*& current)
  {
    if (current == nullptr)
      return;
    else if (key > current->_key)
      remove (key, current->_right);
    else if (key < current->_key)
      remove (key, current->_left);
    else
      {
	if (current->_left != nullptr && current->_right != nullptr)
	  {
	    auto* min_node  = find_min (current->_right);
	    current->_key   = std::move (min_node->_key);
	    current->_value = std::move (min_node->_value);
	    remove (current->_key, current->_right);
	  }
	else
	  {
	    auto* child = current->_left != nullptr ? current->_left : current->_right;
	    delete current;
	    current = child;
	  }
      }
  }

  bool
  contains (i32 key, node* current) const
  {
    if (current == nullptr)
      return false;

    if (key > current->_key)
      return contains (key, current->_right);
    else if (key < current->_key)
      return contains (key, current->_left);
    else
      return true;
  }

  void
  preorder_print (node* current) const
  {
    if (current != nullptr)
      {
	std::cout << current->_key << ' ';
	preorder_print (current->_left);
	preorder_print (current->_right);
      }
  }

  void
  inorder_print (node* current) const
  {
    if (current != nullptr)
      {
	inorder_print (current->_left);
	std::cout << current->_key << ' ';
	inorder_print (current->_right);
      }
  }

  void
  postorder_print (node* current) const
  {
    if (current != nullptr)
      {
	postorder_print (current->_left);
	postorder_print (current->_right);
	std::cout << current->_key << ' ';
      }
  }

  node*
  find_min (node* current) const
  {
    while (current->_left != nullptr)
      current = current->_left;

    return current;
  }

  node* _root;
};

// class binary_search_tree final
// {
// public:
//   binary_search_tree ()
//     : _root {nullptr}
//   {}

//   ~binary_search_tree ()
//   {
//     make_empty (_root);
//   }

//   void
//   insert (int key, int value)
//   {
//     insert (key, value, _root);
//   }

//   void
//   remove (int key)
//   {
//     remove (key, _root);
//   }

//   bool
//   contains (int key) const
//   {
//     return contains (key, _root);
//   }

//   bool
//   empty () const
//   {
//     return _root == nullptr;
//   }

//   void
//   preorder_print () const
//   {
//     preorder_print (_root);
//   }

//   void
//   inorder_print () const
//   {
//     inorder_print (_root);
//   }

//   void
//   postorder_print () const
//   {
//     postorder_print (_root);
//   }

// private:
//   struct node final
//   {
//     node (node* left, node* right, int key, int value)
//       : _left  {left},
// 	_right {right},
// 	_key   {key},
// 	_value {value}
//     {}

//     node* _left;
//     node* _right;
//     int   _key;
//     int   _value;
//   };

//   node*
//   find_min (node* current)
//   {
//     while (current->_left != nullptr)
//       current = current->_left;

//     return current;
//   }

//   void
//   insert (int key, int value, node*& root)
//   {
//     if (root == nullptr)
//       root = new node (nullptr, nullptr, key, value);

//     if (key > root->_key)
//       insert (key, value, root->_right);
//     else if (key < root->_key)
//       insert (key, value, root->_left);
//     else
//       return;			// duplicated, don't do anything
//   }

//   void
//   remove (int key, node*& root)
//   {
//     if (root == nullptr)
//       return;			// doesn't exist

//     if (key > root->_key)
//       remove (key, root->_right);
//     else if (key < root->_key)
//       remove (key, root->_left);
//     else
//       {
// 	if (root->_left != nullptr && root->_right != nullptr)
// 	  {
// 	    auto* min = find_min (root->_right);
// 	    root->_key   = std::move (min->_key);
// 	    root->_value = std::move (min->_value);
// 	    remove (root->_key, root->_right);
// 	  }
// 	else
// 	  {
// 	    node* child = root->_left != nullptr ? root->_left : root->_right;
// 	    delete root;
// 	    root = child;
// 	  }
//       }
//   }

//   void
//   preorder_print (node* root) const
//   {
//     if (root != nullptr)
//       {
// 	std::cout << root->_value << ' ';
// 	preorder_print (root->_left);
// 	preorder_print (root->_right);
//       }
//   }

//   void
//   inorder_print (node* root) const
//   {
//     if (root != nullptr)
//       {
// 	inorder_print (root->_left);
// 	std::cout << root->_value << ' ';
// 	inorder_print (root->_right);
//       }
//   }

//   void
//   postorder_print (node* root) const
//   {
//     if (root != nullptr)
//       {
// 	postorder_print (root->_left);
// 	postorder_print (root->_right);
// 	std::cout << root->_value << ' ';
//       }
//   }

//   bool
//   contains (int key, node* root) const
//   {
//     if (root == nullptr)
//       return false;

//     if (key > root->_key)
//       return contains (key, root->_right);
//     if (key < root->_key)
//       return contains (key, root->_left);
//     else
//       return true;
//   }

//   void
//   make_empty (node* root)
//   {
//     if (root != nullptr)
//       {
// 	make_empty (root->_left);
// 	make_empty (root->_right);
// 	delete root;
//       }
//   }

//   node* _root;
// };

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
