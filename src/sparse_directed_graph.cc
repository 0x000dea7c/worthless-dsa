#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <queue>

class sparse_directed_graph final
{
  std::vector<std::string> _vertices;
  std::unordered_map<std::string, std::vector<std::string>> _list;

public:
  bool add_vertex (std::string const &vertex)
  {
    assert (!vertex.empty ());
    if (_list.find (vertex) != _list.end ())
      {
        return false;
      }
    _vertices.emplace_back (vertex);
    _list[vertex] = std::vector<std::string> ();
    return true;
  }

  bool remove_vertex (std::string const &vertex)
  {
    assert (!vertex.empty ());
    if (_list.find (vertex) == _list.end ())
      {
        return false;
      }
    _vertices.erase (std::remove (_vertices.begin (), _vertices.end (), vertex));
    _list.erase (vertex);
    for (auto &[_, v] : _list)
      {
        v.erase (std::remove (v.begin (), v.end (), vertex), v.end ());
      }
    return true;
  }

  bool add_edge (std::string const &src, std::string const &dst)
  {
    assert (!src.empty ());
    assert (!dst.empty ());
    auto src_it = _list.find (src);
    if (src_it == _list.end () || _list.find (dst) == _list.end ())
      {
        return false;
      }
    auto &vec = src_it->second;
    if (std::find (vec.begin (), vec.end (), dst) != vec.end ())
      {
        return false;
      }
    vec.emplace_back (dst);
    return true;
  }

  bool remove_edge (std::string const &src, std::string const &dst)
  {
    assert (!src.empty ());
    assert (!dst.empty ());
    auto src_it = _list.find (src);
    if (src_it == _list.end () || _list.find (dst) == _list.end ())
      {
        return false;
      }
    auto &vec = src_it->second;
    vec.erase (std::remove (vec.begin (), vec.end (), dst), vec.end ());
    return true;
  }

  void dfs_helper (std::string const &vertex, std::unordered_set<std::string> &visited) const
  {
    visited.emplace (vertex);
    std::cout << vertex << ' ';
    for (auto const &edge : _list.at (vertex))
      {
        if (visited.count (edge) == 0)
          {
            dfs_helper (edge, visited);
          }
      }
  }

  void dfs () const
  {
    std::unordered_set<std::string> visited;
    for (auto const &vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            dfs_helper (vertex, visited);
          }
      }
    std::cout << '\n';
  }

  void bfs () const
  {
    std::queue<std::string> curr_vertices;
    std::unordered_set<std::string> visited;
    for (auto const &vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            visited.emplace (vertex);
            curr_vertices.emplace (vertex);

            while (!curr_vertices.empty ())
              {
                auto curr = curr_vertices.front ();
                curr_vertices.pop ();
                std::cout << curr << ' ';
                for (auto const &edge : _list.at (curr))
                  {
                    if (visited.count (edge) == 0)
                      {
                        visited.emplace (edge);
                        curr_vertices.emplace (edge);
                      }
                  }
              }
          }
      }
    std::cout << '\n';
  }

  bool has_cycle_helper (std::string const &vertex, std::unordered_set<std::string> &visited,
                         std::unordered_set<std::string> &path) const
  {
    visited.emplace (vertex);
    path.emplace (vertex);
    for (auto const &edge : _list.at (vertex))
      {
        if (visited.count (edge) == 0)
          {
            if (has_cycle_helper (edge, visited, path))
              {
                return true;
              }
          }
        else if (path.count (edge) > 0)
          {
            return true;
          }
      }
    path.erase (vertex);
    return false;
  }

  bool has_cycle () const
  {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> path;
    for (auto const &vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            if (has_cycle_helper (vertex, visited, path))
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
    auto src_it = _list.find (src);
    if (src_it == _list.end ())
      {
        return false;
      }
    auto &vec = src_it->second;
    return std::find (vec.begin (), vec.end (), dst) != vec.end ();
  }

  bool empty () const { return _vertices.empty (); }
};

int
main ()
{
  using namespace std::string_literals;

  {
    // Connected regular graph with no cycles.
    sparse_directed_graph graph;

    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_vertex ("C"s);
    graph.add_vertex ("D"s);

    assert (!graph.has_edge ("A"s, "B"s));
    assert (!graph.has_edge ("B"s, "C"s));
    assert (!graph.has_edge ("C"s, "D"s));
    assert (!graph.has_edge ("D"s, "A"s));
    assert (!graph.has_edge ("F"s, "Z"s));

    // A->B->C->D
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "C"s);
    graph.add_edge ("C"s, "D"s);

    assert (graph.has_edge ("A"s, "B"s));
    assert (graph.has_edge ("B"s, "C"s));
    assert (graph.has_edge ("C"s, "D"s));

    assert (!graph.has_edge ("B"s, "A"s));
    assert (!graph.has_edge ("C"s, "B"s));
    assert (!graph.has_edge ("D"s, "C"s));

    assert (!graph.has_cycle ());

    std::cout << "...Printing DFS... Should be: A B C D\n";
    graph.dfs ();

    std::cout << "...Printing BFS... Should be: A B C D\n";
    graph.bfs ();
  }

  {
    // Unconnected regular graph with no cycles.
    sparse_directed_graph graph;

    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_vertex ("C"s);
    graph.add_vertex ("D"s);
    graph.add_vertex ("X"s);

    graph.add_vertex ("F"s);
    graph.add_vertex ("G"s);

    assert (!graph.has_edge ("A"s, "B"s));
    assert (!graph.has_edge ("B"s, "C"s));
    assert (!graph.has_edge ("C"s, "D"s));
    assert (!graph.has_edge ("D"s, "A"s));
    assert (!graph.has_edge ("F"s, "Z"s));

    //    X
    //    ^
    // A->B->C->D
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "C"s);
    graph.add_edge ("B"s, "X"s);
    graph.add_edge ("C"s, "D"s);

    // F->G
    graph.add_edge ("F"s, "G"s);

    assert (graph.has_edge ("A"s, "B"s));
    assert (graph.has_edge ("B"s, "C"s));
    assert (graph.has_edge ("B"s, "X"s));
    assert (graph.has_edge ("C"s, "D"s));

    assert (graph.has_edge ("F"s, "G"s));

    assert (!graph.has_edge ("B"s, "A"s));
    assert (!graph.has_edge ("C"s, "B"s));
    assert (!graph.has_edge ("D"s, "C"s));

    assert (!graph.has_edge ("G"s, "F"s));

    assert (!graph.has_cycle ());

    std::cout << "...Printing DFS... Should be: A B C D X F G\n";
    graph.dfs ();

    std::cout << "...Printing DFS... Should be: A B C X D F G\n";
    graph.bfs ();

    // Add a cycle
    graph.add_edge ("X"s, "A"s);

    assert (graph.has_cycle ());
  }

  {
    // Test empty graph
    sparse_directed_graph graph;
    assert (graph.empty ());
    assert (!graph.has_cycle ());
  }

  {
    // Test adding duplicate vertices
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("A"s); // Duplicate
    graph.add_vertex ("B"s);
    assert (!graph.empty ());
    assert (!graph.has_edge ("A"s, "B"s));
  }

  {
    // Test adding and removing edges
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_edge ("A"s, "B"s);
    assert (graph.has_edge ("A"s, "B"s));
    graph.remove_edge ("A"s, "B"s);
    assert (!graph.has_edge ("A"s, "B"s));
  }

  {
    // Test removing vertices
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_edge ("A"s, "B"s);
    graph.remove_vertex ("A"s);
    assert (!graph.has_edge ("A"s, "B"s));
    assert (!graph.has_edge ("B"s, "A"s));
    assert (!graph.has_cycle ());
  }

  {
    // Test graph with a cycle
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_vertex ("C"s);
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "C"s);
    graph.add_edge ("C"s, "A"s); // Cycle
    assert (graph.has_cycle ());
  }

  {
    // Test disconnected graph
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_vertex ("C"s);
    graph.add_vertex ("D"s);
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("C"s, "D"s);
    assert (!graph.has_edge ("A"s, "C"s));
    assert (!graph.has_cycle ());
  }

  {
    // Test self-loop
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_edge ("A"s, "A"s); // Self-loop
    assert (graph.has_cycle ());
  }

  {
    // Test large graph
    sparse_directed_graph graph;
    for (int i = 0; i < 1000; ++i)
      {
        graph.add_vertex ("V" + std::to_string (i));
      }
    for (int i = 0; i < 999; ++i)
      {
        graph.add_edge ("V" + std::to_string (i), "V" + std::to_string (i + 1));
      }
    assert (!graph.has_cycle ());
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
