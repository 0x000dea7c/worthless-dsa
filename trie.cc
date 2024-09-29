#include <cassert>
#include <iostream>
#include <array>
#include <string>
#include <stack>
#include <memory>

int constexpr alphabet_size {26};

class trie
{
public:
  trie ()
  {
  }

  ~trie ()
  {
  }

  void
  insert (std::string const& key)
  {
    if (key.empty ())
      return;

    if (empty ())
      _root = std::make_shared<node> (false);

    node* current = _root.get ();

    for (auto const& c : key)
      {
	int child {c - 'a'};

	if (current->_children[child] == nullptr)
	  current->_children[child] = std::make_shared<node> (false);

	current = current->_children[child].get ();
      }

    current->_finished = true;
  }

  void
  remove (std::string const& key)
  {
    if (empty () || key.empty ())
      return;

    std::stack<std::shared_ptr<node>> nodes;
    nodes.push (_root);

    for (auto const& c : key)
      {
	auto cn = nodes.top ();

	int child {c - 'a'};

	if (cn->_children[child] == nullptr) // key is not in trie
	  return;

	nodes.push (cn->_children[child]);
      }

    auto cn = nodes.top ();

    if (!cn->_finished)
      return;

    if (! is_leaf (cn))
      {
	cn->_finished = false;
	return;
      }

    while (! nodes.empty ())
      {
	auto cn = nodes.top ();

	nodes.pop ();

	if (cn != nullptr && is_leaf (cn))
	  {
	    if (! nodes.empty ())
	      {
		auto parent = nodes.top ();

		int child {key[nodes.size () - 1] - 'a'};

		parent->_children[child].reset ();

		cn.reset ();
	      }
	  }
      }
  }

  bool
  contains_key (std::string const& key) const
  {
    if (empty () || key.empty ())
      return false;

    std::shared_ptr<node> current {_root};

    for (auto const& c : key)
      {
	int child {c - 'a'};

	if (current->_children[child] == nullptr)
	  return false;

	current = current->_children[child];
      }

    return current->_finished == true;
  }

  bool
  contains_prefix (std::string const& prefix) const
  {
    if (empty () || prefix.empty ())
      return false;

    std::shared_ptr<node> current {_root};

    for (auto const& c : prefix)
      {
	int child {c - 'a'};

	if (current->_children[child] == nullptr)
	  return false;

	current = current->_children[child];
      }

    return true;
  }

  bool
  empty () const
  {
    return is_leaf (_root);
  }

private:
  struct node
  {
    node (bool finished)
      : _finished {finished}
    {}

    std::array<std::shared_ptr<node>, alphabet_size> _children;
    bool _finished;
  };

  bool
  is_leaf (std::shared_ptr<node> const& n) const
  {
    if (n == nullptr)
      return true;

    for (auto const& p : n->_children)
      if (p != nullptr)
	return false;

    return true;
  }

  std::shared_ptr<node> _root;
};

// class trie
// {
// public:
//   trie ()
//     : _root {nullptr},
//       _size {0}
//   {}

//   ~trie ()
//   {
//     remove_root ();
//   }

//   void insert (std::string const& key);

//   bool contains_key (std::string const& key) const;

//   bool contains_prefix (std::string const& prefix) const;

//   void remove (std::string const& key);

//   bool
//   empty () const
//   {
//     return _size == 0;
//   }

// private:
//   struct node
//   {
//     node (bool finished)
//       : _children {},
// 	_finished {finished}
//     {}

//     std::array<node*, alphabet_size> _children;
//     bool _finished;
//   };

//   bool
//   is_leaf (node* root) const
//   {
//     for (int i = 0; i < root->_children.size (); ++i)
//       if (root->_children[i] != nullptr)
// 	return false;

//     return true;
//   }

//   void remove_root ();

//   node* _root;
//   int _size;
// };

// void
// trie::insert (std::string const& key)
// {
//   if (key.empty ())
//     return;

//   if (empty ())
//     {
//       _root = new node (false);
//       ++_size;
//     }

//   std::stack<node*> nodes;
//   nodes.push (_root);

//   for (auto const& c : key)
//     {
//       int child {c - 'a'};

//       auto* n = nodes.top ();

//       nodes.pop ();

//       if (n->_children[child] == nullptr)
// 	{
// 	  n->_children[child] = new node (false);
// 	  ++_size;
// 	}

//       nodes.push (n->_children[child]);
//     }

//   nodes.top ()->_finished = true;
// }

// bool
// trie::contains_key (std::string const& key) const
// {
//   if (key.empty () || empty ())
//     return false;

//   std::stack<node*> nodes;
//   nodes.push (_root);

//   for (auto const& c : key)
//     {
//       int child {c - 'a'};

//       auto* n = nodes.top ();

//       nodes.pop ();

//       if (n->_children[child] == nullptr)
// 	return false;

//       nodes.push (n->_children[child]);
//     }

//   return nodes.top ()->_finished;
// }

// bool
// trie::contains_prefix (std::string const& key) const
// {
//   if (key.empty () || empty ())
//     return false;

//   std::stack<node*> nodes;
//   nodes.push (_root);

//   for (auto const& c : key)
//     {
//       int child {c - 'a'};

//       auto* n = nodes.top ();

//       nodes.pop ();

//       if (n->_children[child] == nullptr)
// 	return false;

//       nodes.push (n->_children[child]);
//     }

//   return nodes.top ()->_finished == false;
// }

// void
// trie::remove (std::string const& key)
// {
//   if (key.empty () || empty ())
//     return;

//   std::stack<node*> nodes;
//   nodes.push (_root);

//   for (auto const& c : key)
//     {
//       int child {c - 'a'};

//       auto* n = nodes.top ();

//       if (n->_children[child] == nullptr)
// 	return;

//       nodes.push (n->_children[child]);
//     }

//   if (nodes.top ()->_finished == false)
//     return;

//   if (is_leaf (nodes.top ()))
//     {
//       while (! nodes.empty () && is_leaf (nodes.top ()))
// 	{
// 	  auto* child = nodes.top ();
// 	  nodes.pop ();

// 	  if (! nodes.empty ())
// 	    {
// 	      auto* parent = nodes.top ();
// 	      int child_index {key[nodes.size () - 1] - 'a'};
// 	      parent->_children[child_index] = nullptr;
// 	    }

// 	  delete child;
// 	  --_size;
// 	}
//     }
//   else
//     nodes.top ()->_finished = false;
// }

// void
// trie::remove_root ()
// {
//   if (empty ())
//     return;

//   std::stack<node*> nodes;
//   nodes.push (_root);

//   while (! nodes.empty ())
//     {
//       auto* c = nodes.top ();

//       nodes.pop ();

//       for (auto* child : c->_children)
// 	if (child != nullptr)
// 	  nodes.push (child);

//       delete c;
//       --_size;
//     }

//   _root = nullptr;
// }

int
main ()
{
  using namespace std::string_literals;

  trie t;

  // Test inserting and searching for single characters
  t.insert ("a"s);
  t.insert ("b"s);
  assert (t.contains_key ("a"s));
  assert (t.contains_key ("b"s));
  assert (! t.contains_key ("c"s));

  // Test inserting and searching for overlapping words
  t.insert ("apple"s);
  t.insert ("app"s);
  assert (t.contains_key ("apple"s));
  assert (t.contains_key ("app"s));
  assert (t.contains_prefix ("ap"s));
  assert (!t.contains_key ("ap"s));

  // Test removing non-existent words
  t.remove ("banana"s); // Should not affect the trie
  assert (t.contains_key ("apple"s));
  assert (t.contains_key ("app"s));

  // Test removing words with shared prefixes
  t.remove ("app"s);
  assert (! t.contains_key ("app"s));
  assert (t.contains_key ("apple"s)); // "apple" should still exist

  // Test edge cases
  t.insert (""s); // Insert empty string
  assert (! t.contains_key (""s)); // Should not be found

  t.insert ("test"s);
  t.remove ("test"s);
  t.remove ("test"s); // Remove twice
  assert(! t.contains_key("test"s));

  // Additional complex cases
  t.insert ("trie"s);
  t.insert ("trigger"s);
  t.insert ("trip"s);
  assert (t.contains_prefix ("tri"s));
  assert (t.contains_key ("trie"s));
  assert (t.contains_key ("trigger"s));
  assert (t.contains_key ("trip"s));

  t.remove ("trie"s);
  assert(! t.contains_key ("trie"s));
  assert( t.contains_key ("trigger"s));
  assert( t.contains_key ("trip"s));

  trie t2;

  t2.insert ("blah"s);
  t2.remove ("blah"s);

  assert (t2.empty ());

  return 0;
}
