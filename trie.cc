#include <cassert>
#include <iostream>
#include <array>
#include <string>
#include <stack>
#include <memory>

int constexpr alphabet_size {26};

// only supports lowercase words! no spaces in between!
class trie
{
public:
  trie ()
    : _root {nullptr}
  {
  }

  ~trie ()
  {
    if (_root == nullptr)
      return;

    std::stack<node*> nodes;
    nodes.push (_root);

    while (! nodes.empty ())
      {
	auto* node = nodes.top ();

	nodes.pop ();

	for (int i = 0; i < node->_children.size (); ++i)
	  {
	    if (node->_children[i] != nullptr && is_leaf (node->_children[i]))
	      {
		delete node->_children[i];
		node->_children[i] = nullptr;
	      }
	    else if (node->_children[i] != nullptr)
	      nodes.push (node->_children[i]);
	  }

	delete node;
      }
  }

  void
  insert (std::string const& key)
  {
    if (key.empty ())
      return;

    if (empty ())
      _root = new node (false);

    node* current {_root};

    for (auto const& c : key)
      {
	int child {c - 'a'};

	if (current->_children[child] == nullptr)
	  current->_children[child] = new node (false);

	current = current->_children[child];
      }

    current->_finished = true;
  }

  void
  remove (std::string const& key)
  {
    if (empty () || key.empty ())
      return;

    std::stack<node*> nodes;
    nodes.push (_root);

    for (auto const& c : key)
      {
	auto* cn = nodes.top ();

	int child {c - 'a'};

	if (cn->_children[child] == nullptr) // key is not in trie
	  return;

	nodes.push (cn->_children[child]);
      }

    if (! nodes.top ()->_finished)
      return;

    if (! is_leaf (nodes.top ()))
      {
	nodes.top ()->_finished = false;
	return;
      }

    while (! nodes.empty ())
      {
	auto* cn = nodes.top ();

	nodes.pop ();

	if (cn != nullptr && is_leaf (cn))
	  {
	    if (! nodes.empty ())
	      {
		auto* parent = nodes.top ();

		int child {key[nodes.size () - 1] - 'a'};

		parent->_children[child] = nullptr;

		delete cn;
	      }
	    else
	      {
		delete _root;
		_root = nullptr;
	      }
	  }
      }
  }

  bool
  contains_key (std::string const& key) const
  {
    if (empty () || key.empty ())
      return false;

    node* current {_root};

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

    node* current {_root};

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
    {
      for (auto*& n : _children)
	n = nullptr;
    }

    std::array<node*, alphabet_size> _children;
    bool _finished;
  };

  bool
  is_leaf (node* n) const
  {
    if (n == nullptr)
      return true;

    for (auto const& p : n->_children)
      if (p != nullptr)
	return false;

    return true;
  }

  node* _root;
};

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

  trie t3;			// leak check
  t3.insert ("imstupid"s);
  t3.insert ("imnot"s);
  t3.insert ("smart"s);
  t3.insert ("but"s);
  t3.insert ("itry"s);
  t3.insert ("mybest"s);

  return 0;
}
