#include <cassert>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <memory>
#include <stack>
#include <queue>

class sparse_directed_graph final
{
public:
  sparse_directed_graph () = default;

  ~sparse_directed_graph () = default;

  bool
  add_vertex (std::string const& vertex)
  {
    if (vertex.empty ())
      {
        return false;
      }

    _vertices.emplace_back (vertex);
    _list[vertex] = std::vector<std::string> ();

    return true;
  }

  bool
  remove_vertex (std::string const& vertex)
  {
    if (vertex.empty ())
      {
        return false;
      }

    std::erase_if (_vertices, [&vertex] (std::string const& arg) { return arg == vertex; });

    _list.erase (vertex);

    for (auto& [_, neighbours] : _list)
      {
        std::erase_if (neighbours, [&vertex] (std::string const& arg) { return arg == vertex; });
      }

    return true;
  }

  bool
  add_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty ())
      {
        return false;
      }

    if (_list.count (src) == 0 || _list.count (dst) == 0)
      {
        return false;
      }

    if (std::find (_list[src].begin (), _list[src].end (), dst) != _list[src].end ())
      {
        return false;
      }

    _list[src].emplace_back (dst);

    return true;
  }

  bool
  remove_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty ())
      {
        return false;
      }

    if (_list.count (src) == 0 || _list.count (dst) == 0)
      {
        return false;
      }

    auto erased = std::erase_if (_list[src], [&dst] (std::string const& arg) { return arg == dst; });

    return erased > 0 ? true : false;
  }

  bool
  has_edge (std::string const& src, std::string const& dst) const
  {
    if (src.empty () || dst.empty ())
      {
        return false;
      }

    if (_list.count (src) == 0 || _list.count (dst) == 0)
      {
        return false;
      }

    return std::find (_list.at (src).begin (), _list.at (src).end (), dst) != _list.at (src).end ();
  }

  void
  dfs_helper (std::string const& vertex, std::unordered_set<std::string>& visited) const
  {
    visited.emplace (vertex);

    std::cout << vertex << ' ';

    for (auto const& edge : _list.at (vertex))
      {
        if (visited.count (edge) == 0)
          {
            dfs_helper (edge, visited);
          }
      }
  }

  void
  dfs () const
  {
    std::unordered_set<std::string> visited;

    for (auto const& vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            dfs_helper (vertex, visited);
          }
      }

    std::cout << '\n';
  }

  void
  bfs () const
  {
    std::queue<std::string> current_vertices;
    std::unordered_set<std::string> visited;

    for (auto const& vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            visited.emplace (vertex);
            current_vertices.emplace (vertex);

            while (! current_vertices.empty ())
              {
                auto current_vertex = current_vertices.front ();

                std::cout << current_vertex << ' ';

                current_vertices.pop ();

                for (auto const& edge : _list.at (current_vertex))
                  {
                    if (visited.count (edge) == 0)
                      {
                        visited.emplace (edge);
                        current_vertices.emplace (edge);
                      }
                  }
              }
          }
      }

    std::cout << '\n';
  }

  bool
  has_cycle_helper (std::string const& vertex, std::unordered_set<std::string>& visited, std::unordered_set<std::string>& path) const
  {
    visited.emplace (vertex);
    path.emplace (vertex);

    for (auto const& edge : _list.at (vertex))
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

  bool
  has_cycle () const
  {
    std::unordered_set<std::string> visited, path;

    for (auto const& vertex : _vertices)
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

  bool
  empty () const
  {
    return _vertices.empty ();
  }

private:
  std::vector<std::string> _vertices;
  std::unordered_map<std::string, std::vector<std::string>> _list;
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

    assert (! graph.has_edge ("A"s, "B"s));
    assert (! graph.has_edge ("B"s, "C"s));
    assert (! graph.has_edge ("C"s, "D"s));
    assert (! graph.has_edge ("D"s, "A"s));
    assert (! graph.has_edge ("F"s, "Z"s));

    // A->B->C->D
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "C"s);
    graph.add_edge ("C"s, "D"s);

    assert (graph.has_edge ("A"s, "B"s));
    assert (graph.has_edge ("B"s, "C"s));
    assert (graph.has_edge ("C"s, "D"s));

    assert (! graph.has_edge ("B"s, "A"s));
    assert (! graph.has_edge ("C"s, "B"s));
    assert (! graph.has_edge ("D"s, "C"s));

    assert (! graph.has_cycle ());

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

    assert (! graph.has_edge ("A"s, "B"s));
    assert (! graph.has_edge ("B"s, "C"s));
    assert (! graph.has_edge ("C"s, "D"s));
    assert (! graph.has_edge ("D"s, "A"s));
    assert (! graph.has_edge ("F"s, "Z"s));

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

    assert (! graph.has_edge ("B"s, "A"s));
    assert (! graph.has_edge ("C"s, "B"s));
    assert (! graph.has_edge ("D"s, "C"s));

    assert (! graph.has_edge ("G"s, "F"s));

    assert (! graph.has_cycle ());

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
    assert (graph.empty());
    assert (! graph.has_cycle());
  }

  {
    // Test adding duplicate vertices
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("A"s); // Duplicate
    graph.add_vertex ("B"s);
    assert (! graph.empty());
    assert (! graph.has_edge("A"s, "B"s));
  }

  {
    // Test adding and removing edges
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_edge ("A"s, "B"s);
    assert (graph.has_edge ("A"s, "B"s));
    graph.remove_edge ("A"s, "B"s);
    assert (! graph.has_edge ("A"s, "B"s));
  }

  {
    // Test removing vertices
    sparse_directed_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_edge ("A"s, "B"s);
    graph.remove_vertex ("A"s);
    assert (! graph.has_edge ("A"s, "B"s));
    assert (! graph.has_edge ("B"s, "A"s));
    assert (! graph.has_cycle ());
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
    assert(! graph.has_edge ("A"s, "C"s));
    assert(! graph.has_cycle ());
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
	graph.add_vertex("V" + std::to_string(i));
      }
    for (int i = 0; i < 999; ++i)
      {
	graph.add_edge("V" + std::to_string(i), "V" + std::to_string(i + 1));
      }
    assert (! graph.has_cycle());
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
