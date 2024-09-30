#include <cassert>
#include <iostream>
#include <array>
#include <string>
#include <stack>

int constexpr alphabet_size {26};

// only supports lowercase words! no spaces in between!

template<typename T>
concept is_std_string = std::is_same_v<T, std::string>;

class trie final
{
public:
  trie ()
    : _root {nullptr}
  {}

  ~trie ()
  {
    if (_root == nullptr)
      return;

    std::stack<node*> nodes;
    nodes.push (_root);

    while (! nodes.empty ())
      {
	auto* current_node = nodes.top ();

	nodes.pop ();

	for (int i = 0; i < current_node->_children.size (); ++i)
	  {
	    if (current_node->_children[i] != nullptr && is_leaf (current_node->_children[i]))
	      {
		delete current_node->_children[i];
		current_node->_children[i] = nullptr;
	      }
	    else if (current_node->_children[i] != nullptr)
	      {
		nodes.push (current_node->_children[i]);
	      }
	  }

	delete current_node;
      }
  }

  void
  insert (is_std_string auto key)
  {
    if (key.empty ())
      return;

    if (empty ())
      _root = new node (false);

    node* current_node {_root};

    for (auto const& c : key)
      {
	int child {c - 'a'};

	if (current_node->_children[child] == nullptr)
	  current_node->_children[child] = new node (false);

	current_node = current_node->_children[child];
      }

    current_node->_finished = true;
  }

  void
  remove (is_std_string auto key)
  {
    if (key.empty () || empty ())
      return;

    std::stack<node*> nodes;
    nodes.push (_root);

    for (auto const& c : key)
      {
	auto* n = nodes.top ();

	int child {c - 'a'};

	if (n->_children[child] == nullptr) // the key doesn't exist, so you can exit
	  return;

	nodes.push (n->_children[child]);
      }

    if (nodes.top ()->_finished == false) // the word is in the dictionary, but it's not register as a word (part of another one maybe?)
      return;

    if (! is_leaf (nodes.top ()))
      {
	nodes.top ()->_finished = false;
	return;
      }

    while (! nodes.empty () && is_leaf (nodes.top ()))
      {
	auto* child_node = nodes.top ();

	nodes.pop ();

	if (! nodes.empty ())
	  {
	    auto* parent = nodes.top ();
	    int child {key[nodes.size () - 1] - 'a'};
	    parent->_children[child] = nullptr;
	    delete child_node;
	  }
      }
  }

  bool
  contains_key (is_std_string auto key) const
  {
    if (key.empty () || empty ())
      return false;

    node* current_node {_root};

    for (auto const& c : key)
      {
	int child {c - 'a'};

	if (current_node->_children[child] == nullptr)
	  return false;

	current_node = current_node->_children[child];
      }

    return current_node->_finished;
  }

  bool
  contains_prefix (is_std_string auto prefix) const
  {
    if (prefix.empty () || empty ())
      return false;

    node* current_node {_root};

    for (auto const& c : prefix)
      {
	int child {c - 'a'};

	if (current_node->_children[child] == nullptr)
	  return false;

	current_node = current_node->_children[child];
      }

    return true;
  }

  bool
  empty () const
  {
    return is_leaf (_root);
  }

private:
  struct node final
  {
    node (bool finished)
      : _finished {finished}
    {
      for (auto*& p : _children)
	p = nullptr;
    }

    std::array<node*, alphabet_size> _children;
    bool _finished;
  };

  bool
  is_leaf (node* n) const
  {
    if (n == nullptr)
      return true;

    for (auto* child : n->_children)
      if (child != nullptr)
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
