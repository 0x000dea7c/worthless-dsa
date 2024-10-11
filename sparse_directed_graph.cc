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
  sparse_directed_graph ()
  {}

  ~sparse_directed_graph ()
  {}

  void
  add_vertex (std::string const& value)
  {
    if (std::find (_vertices.begin (), _vertices.end (), value) != _vertices.end ())
      return;

    _vertices.emplace_back (value);
    _list[value] = std::vector<std::string> ();
  }

  void
  remove_vertex (std::string const& vertex)
  {
    if (vertex.empty () || empty ())
      return;

    std::erase_if (_vertices, [&vertex](std::string const& a) { return vertex == a; });

    for (auto& [_, edges] : _list)
      std::erase_if (edges, [&vertex](std::string const& a) { return vertex == a; });

    _list.erase (vertex);
  }

  void
  add_edge (std::string const& source, std::string const& destination)
  {
    if (source.empty () || destination.empty () || empty () ||
	std::find (_vertices.begin (), _vertices.end (), source) == _vertices.end () ||
	std::find (_vertices.begin (), _vertices.end (), destination) == _vertices.end ())
      return;

    _list[source].emplace_back (destination);
  }

  void
  remove_edge (std::string const& source, std::string const& destination)
  {
    if (source.empty () || destination.empty () || empty () ||
	std::find (_vertices.begin (), _vertices.end (), source) == _vertices.end () ||
	std::find (_vertices.begin (), _vertices.end (), destination) == _vertices.end ())
      return;

    std::erase_if (_list[source], [&destination](std::string const& a) { return destination == a; });
  }

  bool
  empty () const
  {
    return _vertices.size () == 0;
  }

  bool
  has_edge (std::string const& source, std::string const& destination) const
  {
    if (source.empty () || destination.empty () || empty () ||
	std::find (_vertices.begin (), _vertices.end (), source) == _vertices.end () ||
	std::find (_vertices.begin (), _vertices.end (), destination) == _vertices.end ())
      return false;

    return std::find (_list.at (source).begin (), _list.at (source).end (), destination) != _list.at (source).end ();
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
    visited.reserve (_vertices.size ());

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
    visited.reserve (_vertices.size ());
    std::queue<std::string> current_vertices;

    for (auto const& vertex : _vertices)
      {
	if (visited.count (vertex) == 0)
	  {
	    current_vertices.emplace (vertex);
	    visited.emplace (vertex);

	    while (! current_vertices.empty ())
	      {
		auto current_vertex = current_vertices.front ();

		current_vertices.pop ();

		std::cout << current_vertex << ' ';

		for (auto const& edge : _list.at (current_vertex))
		  {
		    if (visited.count (edge) == 0)
		      {
			current_vertices.emplace (edge);
			visited.emplace (edge);
		      }
		  }
	      }
	  }
      }

    std::cout << '\n';
  }

  bool
  has_cycle_helper (std::string const& vertex, std::unordered_set<std::string>& visited, std::unordered_set<std::string>& rec_stack) const
  {
    visited.emplace (vertex);
    rec_stack.emplace (vertex);

    for (auto const& edge : _list.at (vertex))
      {
	if (visited.count (edge) == 0)
	  {
	    if (has_cycle_helper (edge, visited, rec_stack))
	      {
		return true;
	      }
	  }
	else if (rec_stack.count (edge) > 0)
	  {
	    return true;
	  }
      }

    rec_stack.erase (vertex);

    return false;
  }

  bool
  has_cycle () const
  {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> rec_stack;

    for (auto const& vertex : _vertices)
      {
	if (visited.count (vertex) == 0)
	  {
	    if (has_cycle_helper (vertex, visited, rec_stack))
	      {
		return true;
	      }
	  }
      }

    return false;
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
