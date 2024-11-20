#include <cassert>
#include <iostream>
#include <cstdint>
#include <optional>

// not a self-balacing binary tree
class binary_search_tree final
{
  struct node final
  {
    node (node *left, node *right, std::string key, std::string value)
      : _left{left}, _right{right}, _key{key}, _value{value}
    {}
    node *_left;
    node *_right;
    std::string _key;
    std::string _value;
  };

  node *_root;
  uint32_t _size;

  node *find_min (node *curr)
  {
    while (curr && curr->_left)
      {
        curr = curr->_left;
      }
    return curr;
  }

  void make_empty (node *curr)
  {
    if (curr)
      {
        make_empty (curr->_left);
        make_empty (curr->_right);
        delete curr;
      }
  }

  void insert (std::string const &key, std::string const &value, node *&curr)
  {
    if (!curr)
      {
        curr = new node (nullptr, nullptr, key, value);
        ++_size;
      }
    else if (key > curr->_key)
      {
        insert (key, value, curr->_right);
      }
    else if (key < curr->_key)
      {
        insert (key, value, curr->_left);
      }
    else
      {
        curr->_value = value;
      }
  }

  bool remove (std::string const &key, node *&curr)
  {
    if (!curr)
      {
        return false;
      }
    else if (key > curr->_key)
      {
        remove (key, curr->_right);
      }
    else if (key < curr->_key)
      {
        remove (key, curr->_left);
      }
    else
      {
        if (curr->_left && curr->_right)
          {
            auto *min_node = find_min (curr->_right);
            curr->_key = min_node->_key;
            curr->_value = min_node->_value;
            return remove (min_node->_key, curr->_right);
          }
        else
          {
            auto *child = (curr->_left) ? curr->_left : curr->_right;
            delete curr;
            curr = child;
          }
      }
    return true;
  }

  bool contains (std::string const &key, node *curr) const
  {
    if (!curr)
      {
        return false;
      }
    else if (key > curr->_key)
      {
        return contains (key, curr->_right);
      }
    else if (key < curr->_key)
      {
        return contains (key, curr->_left);
      }
    else
      {
        return true;
      }
  }

  void preorder_print (node *curr) const
  {
    if (curr)
      {
        std::cout << curr->_key << ' ';
        preorder_print (curr->_left);
        preorder_print (curr->_right);
      }
  }

  void inorder_print (node *curr) const
  {
    if (curr)
      {
        inorder_print (curr->_left);
        std::cout << curr->_key << ' ';
        inorder_print (curr->_right);
      }
  }

  void postorder_print (node *curr) const
  {
    if (curr)
      {
        postorder_print (curr->_left);
        postorder_print (curr->_right);
        std::cout << curr->_key << ' ';
      }
  }

  std::optional<std::string> get (std::string const &key, node *curr) const
  {
    if (!curr)
      {
        return std::nullopt;
      }
    else if (key > curr->_key)
      {
        return get (key, curr->_right);
      }
    else if (key < curr->_key)
      {
        return get (key, curr->_left);
      }
    else
      {
        return curr->_value;
      }
  }

public:
  binary_search_tree () : _root{nullptr}, _size{0} {}

  ~binary_search_tree () { make_empty (_root); }

  void insert (std::string const &key, std::string const &value)
  {
    assert (!key.empty ());
    assert (!value.empty ());
    insert (key, value, _root);
  }

  bool remove (std::string const &key)
  {
    assert (!key.empty ());
    return remove (key, _root);
  }

  bool contains (std::string const &key) const
  {
    assert (!key.empty ());
    return contains (key, _root);
  }

  void preorder_print () const
  {
    preorder_print (_root);
    std::cout << '\n';
  }

  void inorder_print () const
  {
    inorder_print (_root);
    std::cout << '\n';
  }

  void postorder_print () const
  {
    postorder_print (_root);
    std::cout << '\n';
  }

  bool empty () const { return _root == nullptr; }

  std::optional<std::string> get (std::string const &key) const
  {
    assert (!key.empty ());
    return get (key, _root);
  }
};

int
main ()
{
  using namespace std::string_literals;

  {
    // Basic functionality.
    binary_search_tree tree;

    tree.insert ("self"s, "harm"s);
    tree.insert ("basic"s, "attack"s);
    tree.insert ("jump"s, "crouch"s);

    assert (tree.contains ("self"s));
    assert (tree.contains ("basic"s));
    assert (tree.contains ("jump"s));

    assert (tree.get ("self"s) == "harm"s);
    assert (tree.get ("basic"s) == "attack"s);

    assert (tree.get ("jump"s) == "crouch"s);

    assert (!tree.empty ());

    tree.remove ("self"s);
    tree.remove ("basic"s);
    tree.remove ("jump"s);

    assert (tree.empty ());
  }

  {
    // Override value of key.
    binary_search_tree tree;

    tree.insert ("self"s, "harm"s);

    assert (tree.contains ("self"s));

    tree.insert ("self"s, "loathing"s);

    assert (tree.get ("self"s) == "loathing"s);
  }

  {
    // Removing leaves.
    binary_search_tree tree;

    tree.insert ("i"s, "am"s);
    tree.insert ("very"s, "stupid"s);
    tree.insert ("be"s, "sorry"s);

    assert (!tree.empty ());

    assert (tree.contains ("i"s));
    assert (tree.contains ("very"s));
    assert (tree.contains ("be"s));

    assert (tree.remove ("very"s));
    assert (tree.remove ("be"s));

    assert (!tree.empty ());
    assert (tree.get ("i"s) == "am"s);
  }

  {
    // Printing tree in order.
    binary_search_tree tree;
    tree.insert ("hello"s, "to you"s);
    tree.insert ("dialog"s, "test"s);
    tree.insert ("fire"s, "in the hole"s);
    tree.insert ("luna"s, "random"s);
    tree.insert ("kilo"s, "cheese"s);

    std::cout << "\n...Printing preorder...\n";
    tree.preorder_print ();

    std::cout << "\n...Printing inorder...\n";
    tree.inorder_print ();

    std::cout << "\n...Printing postorder...\n";
    tree.postorder_print ();
  }

  {
    // TODO: need test cases for removing node with 2 children and with 1.
  }

  std::cout << "\nAll tests passed!\n";

  return 0;
}
