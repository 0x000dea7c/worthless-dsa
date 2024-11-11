#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <optional>
#include <cassert>
#include <cstdint>
#include <utility>
#include <algorithm>

class hashmap final
{
public:
  hashmap ()
    : _size {0}
  {
    _data.resize (_buckets);
  }

  ~hashmap () = default;

  bool
  put (std::string const& key, std::string const& value)
  {
    if (key.empty () || value.empty ())
      {
        return false;
      }

    auto hash_key = hash_function (key);
    auto& list = _data[hash_key];

    auto it = std::find_if (list.begin (), list.end (), [&key] (std::pair<std::string, std::string> const& arg) {
      return arg.first == key;
    });

    if (it != list.end ())
      {
        it->second = value;
      }
    else
      {
        list.emplace_back (key, value);
        ++_size;
      }

    return true;
  }

  std::optional<std::string>
  get (std::string const& key) const
  {
    if (key.empty ())
      {
        return std::nullopt;
      }

    auto hash_key = hash_function (key);
    auto& list = _data[hash_key];

    for (auto const& [k, v] : list)
      {
        if (k == key)
          {
            return v;
          }
      }

    return std::nullopt;
  }

  bool
  remove (std::string const& key)
  {
    if (key.empty ())
      {
        return false;
      }

    auto hash_key = hash_function (key);
    auto& list = _data[hash_key];

    auto erased = list.remove_if ([&key] (std::pair<std::string, std::string> const& arg) {
      return key == arg.first;
    });

    if (erased != 0)
      {
        --_size;
        return true;
      }

    return false;
  }

  size_t
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
  static int constexpr _buckets {10};

  size_t
  hash_function (std::string const& str) const
  {
    return std::hash<std::string> () (str) % _buckets;
  }

  std::vector<std::list<std::pair<std::string, std::string>>> _data;
  size_t _size;
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
