#include <cassert>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <stack>
#include <cstdint>
#include <utility>

class trie final
{
public:
  trie ()
    : _root {nullptr}
  {}

  ~trie ()
  {
    if (_root == nullptr)
      {
        return;
      }

    std::stack<node*> nodes;
    nodes.push (_root);

    while (! nodes.empty ())
      {
        auto c = nodes.top ();

        nodes.pop ();

        for (auto* p : c->_children)
          {
            if (p != nullptr)
              {
                nodes.push (p);
              }
          }

        delete c;
      }
  }

  void
  insert (std::string const& key)
  {
    if (key.empty ())
      {
        return;
      }

    if (_root == nullptr)
      {
        _root = new node (false);
      }

    node* current_node {_root};
    uint32_t child;

    for (auto const& n : key)
      {
        child = n - 'a';

        if (current_node->_children[child] == nullptr)
          {
            current_node->_children[child] = new node (false);
          }

        current_node = current_node->_children[child];
      }

    current_node->_finished = true;
  }

  bool
  remove (std::string const& key)
  {
    if (key.empty () || empty ())
      {
        return false;
      }

    std::stack<node*> nodes;
    nodes.push (_root);
    uint32_t child;

    for (auto const& n : key)
      {
        auto* current_node = nodes.top ();

        child = n - 'a';

        if (current_node->_children[child] == nullptr)
          {
            return false;
          }

        nodes.push (current_node->_children[child]);
      }

    if (! nodes.top ()->_finished)
      {
        return false;
      }

    if (! is_leaf (nodes.top ()))
      {
        nodes.top ()->_finished = false;
        return true;
      }

    while (! nodes.empty () && is_leaf (nodes.top ()))
      {
        auto* child = nodes.top ();

        nodes.pop ();

        if (! nodes.empty ())
          {
            auto* parent = nodes.top ();
            parent->_children[key[nodes.size () - 1] - 'a'] = nullptr;
            delete child;
          }
      }

    return true;
  }

  bool
  contains_key (std::string const& key) const
  {
    if (key.empty () || empty ())
      {
        return false;
      }

    auto* current_node = _root;
    uint32_t child;

    for (auto const& n : key)
      {
        child = n - 'a';

        if (current_node->_children[child] == nullptr)
          {
            return false;
          }

        current_node = current_node->_children[child];
      }

    return current_node->_finished;
  }

  bool
  contains_prefix (std::string const& prefix) const
  {
    if (prefix.empty () || empty ())
      {
        return false;
      }

    auto* current_node = _root;
    uint32_t child;

    for (auto const& n : prefix)
      {
        child = n - 'a';

        if (current_node->_children[child] == nullptr)
          {
            return false;
          }

        current_node = current_node->_children[child];
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
    std::vector<std::string> words;

    if (prefix.empty () || empty ())
      {
        return words;
      }

    auto* n = _root;
    uint32_t child;

    for (auto const& l : prefix)
      {
        child = l - 'a';

        if (n->_children[child] == nullptr)
          {
            return words;
          }

        n = n->_children[child];
      }

    std::stack<std::pair<node*, std::string>> current;
    current.emplace (std::make_pair (n, prefix));

    while (! current.empty ())
      {
        auto [n, str] = current.top ();

        current.pop ();

        if (n->_finished)
          {
            words.emplace_back (str);
          }
        else
          {
            for (uint32_t i = 0; i < n->_children.size (); ++i)
	      {
		if (n->_children[i] != nullptr)
		  {
		    char letter = static_cast<char>('a' + i);
		    current.emplace (std::make_pair (n->_children[i], str + letter));
		  }
	      }
          }
      }

    return words;
  }

private:
  static uint32_t constexpr alphabet_size {26};

  struct node final
  {
    node (bool finished)
      : _finished {finished}
    {
      for (auto*& p : _children)
        {
          p = nullptr;
        }
    }

    bool _finished;
    std::array<node*, alphabet_size> _children;
  };

  bool
  is_leaf (node* current) const
  {
    if (current == nullptr)
      {
        return true;
      }

    for (auto* p : current->_children)
      {
        if (p != nullptr)
          {
            return false;
          }
      }

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
