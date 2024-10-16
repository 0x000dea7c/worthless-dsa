#include <cassert>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <stack>
#include <cstdint>

using i32 = std::int32_t;

i32 constexpr alphabet_size {26};

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
	auto node = nodes.top ();

	nodes.pop ();

	for (int i = 0; i < node->_children.size (); ++i)
	  if (node->_children[i])
	    nodes.push (node->_children[i]);

	delete node;
      }
  }

  void
  insert (std::string const& key)
  {
    if (_root == nullptr)
      _root = new node (false);

    node* current {_root};

    for (auto const& c : key)
      {
	int child = c - 'a';

	if (!current->_children[child])
	  current->_children[child] = new node (false);

	current = current->_children[child];
      }

    current->_finished = true;
  }

  void
  remove (std::string const& key)
  {
    if (key.empty () || empty ())
      return;

    std::stack<node*> nodes;
    nodes.push (_root);

    for (auto const& c : key)
      {
	int child = c - 'a';

	if (! nodes.top ()->_children[child])
	  return;

	nodes.push (nodes.top ()->_children[child]);
      }

    if (! nodes.top ()->_finished)
      return;

    if (! is_leaf (nodes.top ()))
      {
	nodes.top ()->_finished = false;
	return;
      }

    while (! nodes.empty () && is_leaf (nodes.top ()))
      {
	node* child_ptr = nodes.top ();

	nodes.pop ();

	if (! nodes.empty ())
	  {
	    node* parent = nodes.top ();
	    int letter = key[nodes.size () - 1];
	    int child = letter - 'a';
	    parent->_children[child] = nullptr;
	    delete child_ptr;
	  }
      }
  }

  bool
  contains_key (std::string const& key) const
  {
    if (key.empty () || empty ())
      return false;

    node* current {_root};

    for (auto const& c : key)
      {
	int child = c - 'a';

	if (!current->_children[child])
	  return false;

	current = current->_children[child];
      }

    return current->_finished == true;
  }

  bool
  contains_prefix (std::string const& prefix) const
  {
    if (prefix.empty () || empty ())
      return false;

    node* current {_root};

    for (auto const& c : prefix)
      {
	int child = c - 'a';

	if (!current->_children[child])
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

  std::vector<std::string>
  get_words_with_shared_prefix (std::string const& prefix) const
  {
    if (prefix.empty () || empty ())
      return {};

    node* current {_root};

    for (auto const& c : prefix)
      {
	int child = c - 'a';

	if (!current->_children[child])
	  return {};

	current = current->_children[child];
      }

    std::vector<std::string> words;
    std::stack<std::pair<node*, std::string>> current_nodes;
    current_nodes.emplace (std::make_pair (current, prefix));

    while (! current_nodes.empty ())
      {
	auto [node, str] = current_nodes.top ();

	current_nodes.pop ();

	if (node->_finished)
	  {
	    words.emplace_back (str);
	  }
	else
	  {
	    for (int i = 0; i < node->_children.size (); ++i)
	      {
		if (node->_children[i])
		  {
		    char letter = static_cast<char>('a' + i);
		    current_nodes.emplace (std::make_pair (node->_children[i], str + letter));
		  }
	      }
	  }
      }

    return words;
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
  is_leaf (node* current) const
  {
    if (!current)
      return true;

    for (int i = 0; i < current->_children.size (); ++i)
      if (current->_children[i])
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

  auto shared = t3.get_words_with_shared_prefix ("im"s);
  assert (shared.size () == 2);

  assert (shared[0] == "imstupid"s);
  assert (shared[1] == "imnot"s);

  std::cout << "All test passed!\n";

  return 0;
}
