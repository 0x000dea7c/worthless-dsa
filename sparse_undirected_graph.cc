#include <cassert>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <memory>
#include <queue>

using i32 = std::int32_t;
using u32 = std::uint32_t;

template<typename T>
concept std_string = std::is_same_v<T, std::string>;

struct vertex final
{
  explicit vertex (std_string auto key)
    : _key {key}
  {}

  explicit vertex (vertex& v)
    : _key {v._key}
  {}

  explicit vertex (vertex&& v)
    : _key {std::move (v._key)}
  {}

  std::string _key;
};

class sparse_undirected_graph final
{
public:
  sparse_undirected_graph ()
  {
  }

  ~sparse_undirected_graph ()
  {
  }

  void
  add_vertex (vertex&& v)
  {
    if (v._key.empty () || _vertices.count (v._key) > 0)
      return;

    _vertices[v._key] = std::make_unique<vertex> (v);
    _list[v._key] = std::unordered_set<std::string> ();
  }

  void
  remove_vertex (std_string auto key)
  {
    if (key.empty () || empty () || _vertices.count (key) == 0)
      return;

    _vertices.erase (key);

    _list.erase (key);

    for (auto& [_, neighbours] : _list)
      neighbours.erase (key);
  }

  void
  add_edge (std_string auto source_key, std_string auto destination_key)
  {
    if (source_key.empty () || destination_key.empty () ||
	_vertices.count (source_key) == 0 || _vertices.count (destination_key) == 0)
      return;

    // if it's not in one way, it shouldn't be in the other one cause it's undirected
    _list[source_key].insert (destination_key);
    _list[destination_key].insert (source_key);
  }

  void
  remove_edge (std_string auto source_key, std_string auto destination_key)
  {
    if (source_key.empty () || destination_key.empty ())
      return;

    _list[source_key].erase (destination_key);
    _list[destination_key].erase (source_key);
  }

  void
  print () const
  {
    if (empty ())
      return;

    for (auto const& [key, edges] : _list)
      {
	std::cout << key << "->";

	for (auto const& edge_key : edges)
	  std::cout << edge_key << ' ';

	std::cout << '\n';
      }
  }

  void
  dfs (std_string auto root) const
  {
    if (root.empty () || empty ())
      return;

    if (_vertices.count (root) == 0)
      return;

    std::unordered_set<std::string> visited;
    std::stack<std::string> current_vertices;
    current_vertices.push (root);
    visited.emplace (root);

    while (! current_vertices.empty ())
      {
	auto current = current_vertices.top ();

	current_vertices.pop ();

	std::cout << current << '\n';

	for (auto const& edge : _list.at (current))
	  {
	    if (visited.count (edge) == 0)
	      {
		visited.emplace (edge);
		current_vertices.emplace (edge);
	      }
	  }
      }
  }

  void
  bfs (std_string auto root) const
  {
    if (root.empty () || empty () || _vertices.count (root) == 0)
      return;

    std::unordered_set<std::string> visited;
    std::queue<std::string> current_vertices;
    current_vertices.push (root);
    visited.emplace (root);

    while (! current_vertices.empty ())
      {
	auto current = current_vertices.front ();

	current_vertices.pop ();

	std::cout << current << '\n';

	for (auto const& edge : _list.at (current))
	  {
	    if (visited.count (edge) == 0)
	      {
		visited.emplace (edge);
		current_vertices.emplace (edge);
	      }
	  }
      }
  }

  bool
  empty () const
  {
    return _vertices.empty ();
  }

  bool
  has_cycle (std_string auto root) const
  {
    if (root.empty () || empty () || _vertices.count (root) == 0)
      return false;

    std::unordered_set<std::string> visited;
    std::stack<std::pair<std::string, std::string>> current_vertices;
    current_vertices.emplace (std::make_pair<> (root, ""));
    visited.emplace (root);

    while (! current_vertices.empty ())
      {
	auto [current_vertex, parent_vertex] = current_vertices.top ();

	current_vertices.pop ();

	for (auto const& edge : _list.at (current_vertex))
	  {
	    if (visited.count (edge) == 0)
	      {
		visited.emplace (edge);
		current_vertices.emplace (std::make_pair<> (edge, current_vertex));
	      }
	    else if (edge != parent_vertex) // for mf undirected graphs
	      return true;
	  }
      }

    return false;
  }

private:
  std::unordered_map<std::string, std::unique_ptr<vertex>> _vertices;
  std::unordered_map<std::string, std::unordered_set<std::string>> _list;
};

int
main ()
{
  using namespace std::string_literals;

  sparse_undirected_graph graph;

  // Initial vertices
  graph.add_vertex (vertex ("A"s));
  graph.add_vertex (vertex ("B"s));
  graph.add_vertex (vertex ("C"s));
  graph.add_vertex (vertex ("D"s));

  assert (! graph.empty ());

  // Adding edges
  graph.add_edge ("A"s, "B"s);
  graph.add_edge ("B"s, "C"s);
  graph.add_edge ("C"s, "D"s);

  assert (! graph.has_cycle ("A"s));
  assert (! graph.has_cycle ("B"s));
  assert (! graph.has_cycle ("C"s));

  std::cout << "Printing all vertices with their edges:\n";
  graph.print ();

  std::cout << "... DFS from B ...\n";
  graph.dfs ("B"s);
  std::cout << "... BFS from B ...\n";
  graph.bfs ("B"s);

  // Test: Adding duplicate vertex
  std::cout << "Adding duplicate vertex 'A'\n";
  graph.add_vertex (vertex ("A"s)); // Should not add

  // Test: Adding duplicate edge
  std::cout << "Adding duplicate edge A-B\n";
  graph.add_edge ("A"s, "B"s); // Should not add

  // Test: Removing non-existent vertex
  std::cout << "Removing non-existent vertex 'E'\n";
  graph.remove_vertex ("E"s); // Should do nothing

  // Test: Removing non-existent edge
  std::cout << "Removing non-existent edge A-C\n";
  graph.remove_edge ("A"s, "C"s); // Should do nothing

  // Test: Removing edges and vertices
  graph.remove_edge ("B"s, "C"s);
  std::cout << "Printing after removing edge B-C:\n";
  graph.print ();

  graph.remove_vertex ("D"s);
  graph.remove_vertex ("A"s);

  std::cout << "Printing vertices with their edges after removing vertices D and A:\n";
  graph.print ();

  // Test: Graph should not be empty
  assert (! graph.empty ());

  // Test: Removing all vertices
  graph.remove_vertex ("B"s);
  graph.remove_vertex ("C"s);

  std::cout << "Printing after removing all vertices:\n";
  graph.print ();

  // Test: Graph should be empty
  assert (graph.empty ());

  // Test: Graph cylce
  std::cout << "Creating graph with cycle.\n";
  sparse_undirected_graph graph2;

  graph.add_vertex (vertex ("A"s));
  graph.add_vertex (vertex ("B"s));
  graph.add_vertex (vertex ("C"s));

  graph.add_edge ("A"s, "B"s);
  graph.add_edge ("B"s, "C"s);
  graph.add_edge ("C"s, "A"s);

  std::cout << "Checking that graph has a cycle.\n";
  assert (graph.has_cycle ("A"s));
  assert (graph.has_cycle ("B"s));
  assert (graph.has_cycle ("C"s));

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
