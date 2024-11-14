#include <cassert>
#include <iostream>
#include <stack>
#include <cstdint>

class binary_search_tree final
{
public:
  binary_search_tree () : _root{nullptr}, _size{0} {}

  ~binary_search_tree ()
  {
    if (_root == nullptr)
      {
        return;
      }

    std::stack<node *> nodes;
    nodes.push (_root);

    while (!nodes.empty ())
      {
        auto *n = nodes.top ();

        nodes.pop ();

        if (n->_right)
          nodes.push (n->_right);

        if (n->_left)
          nodes.push (n->_left);

        delete n;
      }
  }

  void insert (std::string const &key, std::string const &value)
  {
    if (key.empty ())
      {
        return;
      }

    insert (key, value, _root);
  }

  bool remove (std::string const &key)
  {
    if (key.empty ())
      {
        return false;
      }

    return remove (key, _root);
  }

  bool contains (std::string const &key) const
  {
    if (key.empty ())
      {
        return false;
      }

    return contains (key, _root);
  }

  bool empty () const { return _root == nullptr; }

  void preorder_print () const { preorder_print (_root); }

  void inorder_print () const { inorder_print (_root); }

  void postorder_print () const { postorder_print (_root); }

  std::optional<std::string> get (std::string const &key) const { return get (key, _root); }

private:
  struct node final
  {
    node (node *left, node *right, std::string const &key, std::string const &value)
      : _left{left}, _right{right}, _key{key}, _value{value}
    {}

    node *_left;
    node *_right;
    std::string _key;
    std::string _value;
  };

  std::optional<std::string> get (std::string const &key, node *root) const
  {
    if (root == nullptr)
      {
        return std::nullopt;
      }
    else if (key > root->_key)
      {
        return get (key, root->_right);
      }
    else if (key < root->_key)
      {
        return get (key, root->_left);
      }
    else
      {
        return root->_value;
      }
  }

  void insert (std::string const &key, std::string const &value, node *&root)
  {
    if (root == nullptr)
      {
        root = new node (nullptr, nullptr, key, value);
      }
    else if (key > root->_key)
      {
        insert (key, value, root->_right);
      }
    else if (key < root->_key)
      {
        insert (key, value, root->_left);
      }
    else
      {
        root->_value = value;
      }
  }

  bool remove (std::string const &key, node *&root)
  {
    if (root == nullptr)
      {
        return false;
      }
    else if (key > root->_key)
      {
        return remove (key, root->_right);
      }
    else if (key < root->_key)
      {
        return remove (key, root->_left);
      }
    else
      {
        if (root->_left && root->_right)
          {
            node *min = find_min (root->_right);
            root->_key = std::move (min->_key);
            root->_value = std::move (min->_value);
            return remove (key, root->_right);
          }
        else
          {
            node *n = (root->_left) ? root->_left : root->_right;
            delete root;
            root = n;
            return true;
          }
      }
  }

  bool contains (std::string const &key, node *root) const
  {
    if (root == nullptr)
      {
        return false;
      }
    else if (key > root->_key)
      {
        return contains (key, root->_right);
      }
    else if (key < root->_key)
      {
        return contains (key, root->_left);
      }
    else
      {
        return true;
      }
  }

  void preorder_print (node *root) const
  {
    if (root)
      {
        std::cout << root->_value << ' ';
        preorder_print (root->_left);
        preorder_print (root->_right);
      }
  }

  void inorder_print (node *root) const
  {
    if (root != nullptr)
      {
        inorder_print (root->_left);
        std::cout << root->_key << ' ';
        inorder_print (root->_right);
      }
  }

  void postorder_print (node *root) const
  {
    if (root != nullptr)
      {
        postorder_print (root->_left);
        postorder_print (root->_right);
        std::cout << root->_key << ' ';
      }
  }

  node *find_min (node *root) const
  {
    while (root->_left != nullptr)
      {
        root = root->_left;
      }
    return root;
  }

  node *_root;
  uint64_t _size;
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
