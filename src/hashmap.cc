#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <optional>
#include <cassert>
#include <cstdint>
#include <utility>

// Only stores strings and the number of buckets is fixed.
class hashmap final
{
public:
  hashmap ()
    : _table (capacity),
      _size {0}
  {}

  ~hashmap () = default;

  void
  put (std::string const& key, std::string const& value)
  {
    auto hash = hash_function (key);
    auto& list = _table[hash];

    for (auto& [k, v] : list)
      {
        if (k == key)
          {
            v = value;
            return;
          }
      }

    _table[hash].emplace_back (key, value);

    ++_size;
  }

  bool
  remove (std::string const& key)
  {
    auto hash = hash_function (key);
    auto& list = _table[hash];

    for (auto it = list.begin (); it != list.end (); ++it)
      {
        if (it->first == key)
          {
            list.erase (it);
            --_size;
            return true;
          }
      }

    return false;
  }

  std::optional<std::string>
  get (std::string const& key) const
  {
    auto hash = hash_function (key);
    auto& list = _table[hash];

    for (auto it = list.begin (); it != list.end (); ++it)
      {
        if (it->first == key)
          {
            return it->second;
          }
      }

    return std::nullopt;
  }

  uint32_t
  size () const
  {
    return _size;
  }

  bool
  empty () const
  {
    return size () == 0;
  }

private:
  uint32_t
  hash_function (std::string const& key) const
  {
    return std::hash<std::string>()(key) % capacity;
  }

  static uint32_t constexpr capacity {10};
  std::vector<std::list<std::pair<std::string, std::string>>> _table;
  uint32_t _size;
};

int
main ()
{
  {
    // Basic functionality.
    hashmap m;
    m.put ("self", "harm");
    m.put ("loathing", "self");
    m.put ("mutilation", "self");
    m.put ("improvement", "self");
    m.put ("deprivation", "self");

    assert (m.size () == 5);
    assert (! m.empty ());

    assert (m.get ("self") == "harm");
    assert (m.get ("loathing") == "self");
    assert (m.get ("mutilation") == "self");
    assert (m.get ("improvement") == "self");
    assert (m.get ("deprivation") == "self");

    assert (m.remove ("self"));
    assert (m.remove ("loathing"));
    assert (m.remove ("mutilation"));
    assert (m.remove ("improvement"));
    assert (m.remove ("deprivation"));

    assert (m.size () == 0);
    assert (m.empty ());
  }

  {
    // Updating existing key.
    hashmap m;
    m.put ("self", "loathing");
    m.put ("self", "harm");

    assert (m.get ("self") == "harm");
    assert (m.size () == 1);
  }

  {
    // Removing non-existing key.
    hashmap m;
    m.put ("dog", "cat");

    assert (! m.remove ("php"));
  }

  {
    // Getting non-existing key.
    hashmap m;
    m.put ("hazardous", "materials");

    // You can check for ex6tenz in these two ways, cool!
    assert (m.get ("HAZMAT") == std::nullopt);
    assert (! m.get ("HAZMAT"));
  }

  {
    // Empty hash map test.
    hashmap m;

    assert (! m.get ("unforeseen consequences"));
    assert (! m.remove ("darkness"));
  }

  {
    // Multiple keys within the same bucket.
    hashmap m;

    m.put ("dog", "one");
    m.put ("dug", "two");
    m.put ("dyg", "three");

    assert (m.size () == 3);
    assert (m.get ("dog") == "one");
    assert (m.get ("dug") == "two");
    assert (m.get ("dyg") == "three");

    assert (m.remove ("dog"));
    assert (m.size () == 2);
    assert (m.get ("dug") == "two");
    assert (m.get ("dyg") == "three");

    assert (m.remove ("dyg"));
    assert (m.size () == 1);
    assert (m.get ("dug") == "two");
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
