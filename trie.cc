#include <cassert>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <stack>
#include <cstdint>

template<typename T>
concept std_string = std::is_same_v<T, std::string>;

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
	auto* top = nodes.top ();

	nodes.pop ();

	for (auto*& c : top->_children)
	  {
	    if (c != nullptr && is_leaf (c))
	      {
		delete c;
	      }
	    else if (c != nullptr)
	      {
		nodes.push (c);
	      }
	  }

	delete top;
      }
  }

  void
  insert (std_string auto key)
  {
    if (key.empty ())
      return;

    if (empty ())
      _root = new node (false);

    node* current {_root};

    for (auto const& c : key)
      {
	i32 child {c - 'a'};

	if (current->_children[child] == nullptr)
	  current->_children[child] = new node (false);

	current = current->_children[child];
      }

    current->_finished = true;
  }

  void
  remove (std_string auto key)
  {
    if (key.empty () || empty ())
      return;

    std::stack<node*> nodes;
    nodes.push (_root);

    for (auto const& c : key)
      {
	auto* current_node = nodes.top ();

	i32 child {c - 'a'};

	if (current_node->_children[child] == nullptr)
	  return;

	nodes.push (current_node->_children[child]);
      }

    if (!nodes.top ()->_finished)
      return;

    if (! is_leaf (nodes.top ()))
      {
	nodes.top ()->_finished = false;
	return;
      }

    while (! nodes.empty () && is_leaf (nodes.top ()))
      {
	auto* child = nodes.top ();

	nodes.pop ();

	if (! nodes.empty ())
	  {
	    auto* parent = nodes.top ();
	    i32 child_index {key[nodes.size () - 1] - 'a'};
	    delete child;
	    parent->_children[child_index] = nullptr;
	  }
      }
  }

  bool
  contains_key (std_string auto key) const
  {
    if (key.empty () || empty ())
      return false;

    node* current {_root};

    for (auto const& c : key)
      {
	i32 child {c - 'a'};

	if (current->_children[child] == nullptr)
	  return false;

	current = current->_children[child];
      }

    return current->_finished == true;
  }

  bool
  contains_prefix (std_string auto prefix) const
  {
    if (prefix.empty () || empty ())
      return false;

    node* current {_root};

    for (auto const& c : prefix)
      {
	i32 child {c - 'a'};

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

  std::vector<std::string>
  get_words_with_shared_prefix (std_string auto prefix) const
  {
    std::vector<std::string> words;

    if (prefix.empty () || empty ())
      return words;

    node* current {_root};

    for (auto const& c : prefix)
      {
	i32 child {c - 'a'};

	if (current->_children[child] == nullptr)
	  return words;

	current = current->_children[child];
      }

    std::stack<std::pair<node*, std::string>> nodes;
    nodes.emplace (std::make_pair (current, prefix));

    while (! nodes.empty ())
      {
	bool is_leaf {true};

	auto [current_node, current_str] = nodes.top ();

	nodes.pop ();

	for (i32 i {0}; i < current_node->_children.size (); ++i)
	  {
	    if (current_node->_children[i] != nullptr)
	      {
		char current_char = static_cast<char> ('a' + i);
		nodes.emplace (current_node->_children[i], current_str + current_char);
		is_leaf = false;
	      }
	  }

	if (is_leaf)
	  {
	    words.emplace_back (current_str);
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
      for (auto*& p: _children)
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

    for (auto* p : n->_children)
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

  auto shared = t3.get_words_with_shared_prefix ("im"s);
  assert (shared.size () == 2);
  assert (shared[0] == "imstupid"s);
  assert (shared[1] == "imnot"s);

  std::cout << "All test passed!\n";

  return 0;
}
