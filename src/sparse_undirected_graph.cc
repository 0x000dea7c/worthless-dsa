#include <cassert>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <queue>

class sparse_undirected_graph final
{
public:
  sparse_undirected_graph () = default;

  ~sparse_undirected_graph () = default;

  void
  add_vertex (std::string const& vertex)
  {
    if (vertex.empty () || _list.find (vertex) != _list.end ())
      return;

    _list[vertex] = std::vector<std::string> ();

    _vertices.emplace_back (vertex);
  }

  void
  remove_vertex (std::string const& vertex)
  {
    if (vertex.empty () || _list.find (vertex) == _list.end ())
      return;

    _list.erase (vertex);

    std::erase_if (_vertices, [&vertex] (std::string const& arg) { return arg == vertex; });

    for (auto& [_, neighbours] : _list)
      {
        std::erase_if (neighbours, [&vertex] (std::string const& arg) { return arg == vertex; });
      }
  }

  void
  add_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty ())
      return;

    auto src_it = _list.find (src);
    auto dst_it = _list.find (dst);

    if (src_it == _list.end () || dst_it == _list.end ())
      return;

    if (std::find (src_it->second.begin (), src_it->second.end (), dst) != src_it->second.end ())
      return;

    src_it->second.emplace_back (dst);
    dst_it->second.emplace_back (src);
  }

  void
  remove_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty ())
      return;

    auto src_it = _list.find (src);
    auto dst_it = _list.find (dst);

    if (src_it == _list.end () || dst_it == _list.end ())
      return;

    std::erase_if (src_it->second, [&dst] (std::string const& arg) { return arg == dst; });
    std::erase_if (dst_it->second, [&src] (std::string const& arg) { return arg == src; });
  }

  bool
  has_edge (std::string const& src, std::string const& dst) const
  {
    if (src.empty () || dst.empty ())
      return false;

    auto src_it = _list.find (src);
    auto dst_it = _list.find (dst);

    if (src_it == _list.end () || dst_it == _list.end ())
      return false;

    return std::find (src_it->second.begin (), src_it->second.end (), dst) != src_it->second.end ();
  }

  bool
  empty () const
  {
    return _vertices.empty ();
  }

  bool
  has_cycle_helper (std::string const& vertex, std::string const& parent, std::unordered_set<std::string>& visited) const
  {
    visited.emplace (vertex);

    for (auto const& edge : _list.at (vertex))
      {
        if (visited.count (edge) == 0)
          {
            if (has_cycle_helper (edge, vertex, visited))
              {
                return true;
              }
          }
        else if (edge != parent)
          {
            return true;
          }
      }

    return false;
  }

  bool
  has_cycle () const
  {
    std::unordered_set<std::string> visited;

    for (auto const& vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            if (has_cycle_helper (vertex, "", visited))
              {
                return true;
              }
          }
      }

    return false;
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
    std::unordered_set<std::string> visited;
    std::queue<std::string> current_vertices;

    for (auto const& vertex : _vertices)
      {
        if (visited.count (vertex) == 0)
          {
            visited.emplace (vertex);
            current_vertices.emplace (vertex);

            while (! current_vertices.empty ())
              {
                auto current_vertex = current_vertices.front ();

                current_vertices.pop ();

                std::cout << current_vertex << ' ';

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

private:
  std::vector<std::string> _vertices;
  std::unordered_map<std::string, std::vector<std::string>> _list;
};

// class sparse_undirected_graph final
// {
// public:
//   sparse_undirected_graph ()  = default;

//   ~sparse_undirected_graph () = default;

//   void
//   add_vertex (std::string const& vertex)
//   {
//     if (vertex.empty () || _list.count (vertex) != 0)
//       return;

//     _vertices.emplace_back (vertex);
//     _list[vertex] = std::vector<std::string> ();
//   }

//   void
//   remove_vertex (std::string const& vertex)
//   {
//     if (vertex.empty () || _list.count (vertex) == 0)
//       return;

//     std::erase_if (_vertices, [&vertex] (std::string const& arg) { return arg == vertex; });

//     _list.erase (vertex);

//     for (auto& [_, neighbours] : _list)
//       std::erase_if (neighbours, [&vertex] (std::string const& arg) { return arg == vertex; });
//   }

//   void
//   add_edge (std::string const& src, std::string const& dst)
//   {
//     if (src.empty () || dst.empty () || _list.count (src) == 0 || _list.count (dst) == 0)
//       return;

//     // no duplicates
//     if (std::find (_list.at (src).begin (), _list.at (src).end (), dst) != _list.at (src).end ())
//       return;

//     _list.at (src).emplace_back (dst);
//     _list.at (dst).emplace_back (src);
//   }

//   void
//   remove_edge (std::string const& src, std::string const& dst)
//   {
//     if (src.empty () || dst.empty () || _list.count (src) == 0 || _list.count (dst) == 0)
//       return;

//     std::erase_if (_list.at (src), [&dst] (std::string const& arg) { return arg == dst; });
//     std::erase_if (_list.at (dst), [&src] (std::string const& arg) { return arg == src; });
//   }

//   bool
//   has_edge (std::string const& src, std::string const& dst) const
//   {
//     if (src.empty () || dst.empty () || _list.count (src) == 0 || _list.count (dst) == 0)
//       return false;

//     return std::find (_list.at (src).begin (), _list.at (src).end (), dst) != _list.at (src).end ();
//   }

//   bool
//   empty () const
//   {
//     return _vertices.empty ();
//   }

//   void
//   dfs_helper (std::string const& vertex, std::unordered_set<std::string>& visited) const
//   {
//     visited.emplace (vertex);

//     std::cout << vertex << ' ';

//     for (auto const& edge : _list.at (vertex))
//       {
// 	if (visited.count (edge) == 0)
// 	  {
// 	    dfs_helper (edge, visited);
// 	  }
//       }
//   }

//   void
//   dfs () const
//   {
//     std::unordered_set<std::string> visited;

//     for (auto const& vertex : _vertices)
//       {
// 	if (visited.count (vertex) == 0)
// 	  dfs_helper (vertex, visited);
//       }

//     std::cout << '\n';
//   }

//   void
//   bfs () const
//   {
//     std::unordered_set<std::string> visited;
//     std::queue<std::string> current_vertices;

//     for (auto const& vertex : _vertices)
//       {
// 	if (visited.count (vertex) == 0)
// 	  {
// 	    visited.emplace (vertex);
// 	    current_vertices.emplace (vertex);

// 	    while (! current_vertices.empty ())
// 	      {
// 		auto current_vertex = current_vertices.front ();

// 		current_vertices.pop ();

// 		std::cout << current_vertex << ' ';

// 		for (auto const& edge : _list.at (current_vertex))
// 		  {
// 		    if (visited.count (edge) == 0)
// 		      {
// 			current_vertices.emplace (edge);
// 			visited.emplace (edge);
// 		      }
// 		  }
// 	      }
// 	  }
//       }

//     std::cout <<  '\n';
//   }

//   bool
//   has_cycle_helper (std::string const& vertex, std::string const& parent, std::unordered_set<std::string>& visited) const
//   {
//     visited.emplace (vertex);

//     for (auto const& edge : _list.at (vertex))
//       {
// 	if (visited.count (edge) == 0)
// 	  {
// 	    if (has_cycle_helper (edge, vertex, visited))
// 	      return true;
// 	  }
// 	else if (edge != parent)
// 	  return true;
//       }

//     return false;
//   }

//   bool
//   has_cycle () const
//   {
//     std::unordered_set<std::string> visited;

//     for (auto const& vertex : _vertices)
//       {
// 	if (visited.count (vertex) == 0)
// 	  {
// 	    if (has_cycle_helper (vertex, "", visited))
// 	      return true;
// 	  }
//       }

//     return false;
//   }

// private:
//   std::vector<std::string> _vertices;
//   std::unordered_map<std::string, std::vector<std::string>> _list;
// };

int
main ()
{
  using namespace std::string_literals;

  {
    // Connected regular graph with no cycles.
    sparse_undirected_graph graph;

    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_vertex ("C"s);
    graph.add_vertex ("D"s);

    assert (! graph.has_edge ("A"s, "B"s));
    assert (! graph.has_edge ("B"s, "C"s));
    assert (! graph.has_edge ("C"s, "D"s));
    assert (! graph.has_edge ("D"s, "A"s));
    assert (! graph.has_edge ("F"s, "Z"s));

    // A<->B<->C<->D
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "C"s);
    graph.add_edge ("C"s, "D"s);

    assert (graph.has_edge ("A"s, "B"s));
    assert (graph.has_edge ("B"s, "A"s));
    assert (graph.has_edge ("B"s, "C"s));
    assert (graph.has_edge ("C"s, "B"s));
    assert (graph.has_edge ("C"s, "D"s));
    assert (graph.has_edge ("D"s, "C"s));

    assert (! graph.has_cycle ());

    std::cout << "...Printing DFS... Should be: A B C D\n";
    graph.dfs ();

    std::cout << "...Printing BFS... Should be: A B C D\n";
    graph.bfs ();
  }

  {
    // Unconnected regular graph with no cycles.
    sparse_undirected_graph graph;

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

    //     X
    //     ^
    // A<->B<->C<->D
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "C"s);
    graph.add_edge ("B"s, "X"s);
    graph.add_edge ("C"s, "D"s);

    // F->G
    graph.add_edge ("F"s, "G"s);

    assert (graph.has_edge ("A"s, "B"s));
    assert (graph.has_edge ("B"s, "A"s));
    assert (graph.has_edge ("B"s, "C"s));
    assert (graph.has_edge ("C"s, "B"s));
    assert (graph.has_edge ("B"s, "X"s));
    assert (graph.has_edge ("X"s, "B"s));
    assert (graph.has_edge ("C"s, "D"s));
    assert (graph.has_edge ("D"s, "C"s));

    assert (graph.has_edge ("F"s, "G"s));
    assert (graph.has_edge ("G"s, "F"s));

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
    sparse_undirected_graph graph;
    assert (graph.empty());
    assert (! graph.has_cycle());
  }

  {
    // Test adding duplicate vertices
    sparse_undirected_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("A"s); // Duplicate
    graph.add_vertex ("B"s);
    assert (! graph.empty());
    assert (! graph.has_edge("A"s, "B"s));
  }

  {
    // Test adding and removing edges
    sparse_undirected_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_edge ("A"s, "B"s);
    assert (graph.has_edge ("A"s, "B"s));
    assert (graph.has_edge ("B"s, "A"s));
    graph.remove_edge ("A"s, "B"s);
    assert (! graph.has_edge ("A"s, "B"s));
    assert (! graph.has_edge ("B"s, "A"s));
  }

  {
    // Test removing vertices
    sparse_undirected_graph graph;
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
    sparse_undirected_graph graph;
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
    sparse_undirected_graph graph;
    graph.add_vertex ("A"s);
    graph.add_vertex ("B"s);
    graph.add_vertex ("C"s);
    graph.add_vertex ("D"s);
    graph.add_edge ("A"s, "B"s);
    graph.add_edge ("B"s, "A"s); // duplicate crap, ignore
    graph.add_edge ("C"s, "D"s);
    graph.add_edge ("D"s, "C"s); // duplicate crap, ignore
    // A<->B C<->D
    assert(! graph.has_edge ("A"s, "C"s));
    assert(! graph.has_cycle ());
  }

  {
    // Test self-loop
    sparse_undirected_graph graph;
    graph.add_vertex ("A"s);
    graph.add_edge ("A"s, "A"s); // Self-loop
    assert (graph.has_cycle ());
  }

  {
    // Test large graph
    sparse_undirected_graph graph;
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
