#include <cassert>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <queue>
#include <cstdint>

//
// assumption: you won't be adding/removing vertices because it's freaking expensive.
// in fact, you need to pass a vector with every vertex created because fuck it
//
class dense_directed_graph final
{
public:
  dense_directed_graph (std::vector<std::string> &data)
  {
    _index_to_key.resize (data.size (), std::string ());
    for (uint32_t i = 0; i < data.size (); ++i)
      {
        _index_to_key[i] = data[i];
        _key_to_index[data[i]] = i;
      }
    _matrix.resize (data.size (), std::vector<bool> (data.size (), false));
  }

  ~dense_directed_graph () = default;

  bool add_edge (std::string const &src, std::string const &dst)
  {
    assert (!src.empty ());
    assert (!dst.empty ());
    auto src_it = _key_to_index.find (src);
    auto dst_it = _key_to_index.find (dst);
    if (src_it == _key_to_index.end () || dst_it == _key_to_index.end ())
      {
        return false;
      }
    auto src_idx = src_it->second;
    auto dst_idx = dst_it->second;
    _matrix[src_idx][dst_idx] = true;
    return true;
  }

  bool remove_edge (std::string const &src, std::string const &dst)
  {
    assert (!src.empty ());
    assert (!dst.empty ());
    auto src_it = _key_to_index.find (src);
    auto dst_it = _key_to_index.find (dst);
    if (src_it == _key_to_index.end () || dst_it == _key_to_index.end ())
      {
        return false;
      }
    auto src_idx = src_it->second;
    auto dst_idx = dst_it->second;
    _matrix[src_idx][dst_idx] = false;
    return true;
  }

  void dfs_helper (uint32_t vertex, std::unordered_set<uint32_t> &visited) const
  {
    visited.emplace (vertex);
    std::cout << _index_to_key.at (vertex) << ' ';
    for (uint32_t i = 0; i < _matrix[vertex].size (); ++i)
      {
        if (_matrix[vertex][i] && visited.count (i) == 0)
          {
            dfs_helper (i, visited);
          }
      }
  }

  void dfs () const
  {
    std::unordered_set<uint32_t> visited;
    for (uint32_t i = 0; i < _matrix.size (); ++i)
      {
        if (visited.count (i) == 0)
          {
            dfs_helper (i, visited);
          }
      }
    std::cout << '\n';
  }

  void bfs () const
  {
    std::queue<uint32_t> current_vertices;
    std::unordered_set<uint32_t> visited;
    for (uint32_t i = 0; i < _matrix.size (); ++i)
      {
        if (visited.count (i) == 0)
          {
            visited.emplace (i);
            current_vertices.emplace (i);
            while (!current_vertices.empty ())
              {
                auto curr = current_vertices.front ();
                current_vertices.pop ();
                std::cout << _index_to_key.at (curr) << ' ';
                for (uint32_t j = 0; j < _matrix.size (); ++j)
                  {
                    if (_matrix[curr][j] && visited.count (j) == 0)
                      {
                        current_vertices.emplace (j);
                        visited.emplace (j);
                      }
                  }
              }
          }
      }
    std::cout << '\n';
  }

  bool has_cycle_helper (uint32_t vertex, std::unordered_set<uint32_t> &visited,
                         std::unordered_set<uint32_t> &path) const
  {
    path.emplace (vertex);
    visited.emplace (vertex);
    for (uint32_t i = 0; i < _matrix[vertex].size (); ++i)
      {
        if (_matrix[vertex][i] && visited.count (i) == 0)
          {
            if (has_cycle_helper (i, visited, path))
              {
                return true;
              }
          }
        else if (_matrix[vertex][i] && path.count (i) > 0)
          {
            return true;
          }
      }
    path.erase (vertex);
    return false;
  }

  bool has_cycle () const
  {
    std::unordered_set<uint32_t> visited;
    std::unordered_set<uint32_t> path;
    for (uint32_t i = 0; i < _matrix.size (); ++i)
      {
        if (visited.count (i) == 0)
          {
            if (has_cycle_helper (i, visited, path))
              {
                return true;
              }
          }
      }
    return false;
  }

  bool has_edge (std::string const &src, std::string const &dst) const
  {
    assert (!src.empty ());
    assert (!dst.empty ());
    auto src_it = _key_to_index.find (src);
    auto dst_it = _key_to_index.find (dst);
    if (src_it == _key_to_index.end () || dst_it == _key_to_index.end ())
      {
        return false;
      }
    auto src_idx = src_it->second;
    auto dst_idx = dst_it->second;
    return _matrix[src_idx][dst_idx];
  }

private:
  std::vector<std::vector<bool>> _matrix;
  std::vector<std::string> _index_to_key;
  std::unordered_map<std::string, uint32_t> _key_to_index;
  uint32_t _index;
};

int
main ()
{
  using namespace std::string_literals;

  // Test 1: Empty graph
  {
    std::vector<std::string> vertices;
    dense_directed_graph g (vertices);
    assert (!g.has_cycle ());
    assert (!g.has_edge ("A"s, "B"s));
  }

  // Test 2: Single vertex graph
  {
    std::vector<std::string> vertices = {"A"s};
    dense_directed_graph g (vertices);
    assert (!g.has_cycle ());
    assert (!g.has_edge ("A"s, "A"s));
  }

  // Test 3: Two connected components
  {
    std::vector<std::string> vertices = {"A"s, "B"s, "C"s, "D"s};
    dense_directed_graph g (vertices);

    g.add_edge ("A"s, "B"s);
    g.add_edge ("C"s, "D"s);

    assert (!g.has_cycle ());

    // Check edges
    assert (g.has_edge ("A"s, "B"s));
    assert (g.has_edge ("C"s, "D"s));
    assert (!g.has_edge ("A"s, "C"s));
    assert (!g.has_edge ("B"s, "D"s));

    // Add cycle
    g.add_edge ("B"s, "A"s);
    assert (g.has_cycle ());

    // Remove cycle
    g.remove_edge ("B"s, "A"s);
    assert (!g.has_cycle ());
  }

  // Test 4: Multiple cycles
  {
    std::vector<std::string> vertices = {"E"s, "F"s, "G"s};
    dense_directed_graph g (vertices);

    g.add_edge ("E"s, "F"s);
    g.add_edge ("F"s, "G"s);
    g.add_edge ("G"s, "E"s); // Creates a cycle E -> F -> G -> E

    assert (g.has_cycle ());

    g.remove_edge ("E"s, "F"s); // resulting in F -> G -> E
    assert (!g.has_cycle ());
  }

  // Test 5: Large graph with multiple connected components
  {
    std::vector<std::string> vertices = {"X"s, "Y"s, "Z"s, "W"s, "V"s};
    dense_directed_graph g (vertices);

    g.add_edge ("X"s, "Y"s);
    g.add_edge ("Y"s, "Z"s);
    g.add_edge ("Z"s, "W"s);
    g.add_edge ("W"s, "V"s);
    g.add_edge ("V"s, "X"s); // Creates a cycle X -> Y -> Z -> W -> V -> X

    assert (g.has_cycle ());

    assert (g.has_edge ("X"s, "Y"s));
    assert (g.has_edge ("Z"s, "W"s));
    assert (g.has_edge ("V"s, "X"s));

    assert (!g.has_edge ("X"s, "Z"s));
  }

  // Test 6: Edge operations
  {
    std::vector<std::string> vertices = {"P"s, "Q"s, "R"s, "S"s};
    dense_directed_graph g (vertices);

    g.add_edge ("P"s, "Q"s);
    g.add_edge ("Q"s, "R"s);
    g.add_edge ("R"s, "S"s);

    assert (g.has_edge ("P"s, "Q"s));
    assert (g.has_edge ("Q"s, "R"s));
    assert (g.has_edge ("R"s, "S"s));
    assert (!g.has_edge ("S"s, "P"s)); // No edge P -> S

    // Remove edge
    g.remove_edge ("P"s, "Q"s);
    assert (!g.has_edge ("P"s, "Q"s));
  }

  // Test 7: DFS and BFS traversal
  {
    std::vector<std::string> vertices = {"T"s, "U"s, "V"s, "W"s};
    dense_directed_graph g (vertices);

    g.add_edge ("T"s, "U"s);
    g.add_edge ("U"s, "V"s);
    g.add_edge ("V"s, "W"s);
    g.add_edge ("W"s, "T"s); // Creates a cycle T -> U -> V -> W -> T

    g.dfs (); // T U V W
    g.bfs (); // T U V W
  }

  {
    // Z <- T -> U -> V -> W
    std::vector<std::string> vertices = {"T"s, "U"s, "V"s, "W"s, "Z"s};
    dense_directed_graph g (vertices);

    g.add_edge ("T"s, "Z"s);
    g.add_edge ("T"s, "U"s);
    g.add_edge ("U"s, "V"s);
    g.add_edge ("V"s, "W"s);

    g.dfs (); // This should be: T U V W Z
    g.bfs (); // This should be: T U Z V W
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
