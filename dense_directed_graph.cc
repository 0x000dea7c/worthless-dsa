#include <cstdint>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <queue>

using i32 = std::int32_t;
using u32 = std::uint32_t;

// assumption: you won't be adding/removing vertices because it's freaking expensive.
// in fact, you need to pass a vector with every vertex created because fuck it
class dense_directed_graph final
{
public:
  dense_directed_graph (std::vector<std::string>& vertices)
    : _vertices {vertices}
  {
    _matrix.resize (_vertices.size (), std::vector<bool> (_vertices.size (), false));

    for (u32 i {}; i < _vertices.size (); ++i)
      {
	_key_to_index[_vertices[i]] = i;
	_index_to_key[i] = _vertices[i];
      }
  }

  ~dense_directed_graph ()
  {}

  template<typename T>
  void
  add_edge (T&& source, T&& destination)
  {
    auto source_it = _key_to_index.find (std::forward<T> (source));
    auto destination_it = _key_to_index.find (std::forward<T> (destination));

    if (source_it == _key_to_index.end () || destination_it == _key_to_index.end ())
      return;

    auto source_id = source_it->second;
    auto destination_id = destination_it->second;

    _matrix[source_id][destination_id] = true;
  }

  template<typename T>
  void
  remove_edge (T&& source, T&& destination)
  {
    auto source_it = _key_to_index.find (std::forward<T> (source));
    auto destination_it = _key_to_index.find (std::forward<T> (destination));

    if (source_it == _key_to_index.end () || destination_it == _key_to_index.end ())
      return;

    auto source_id = source_it->second;
    auto destination_id = destination_it->second;

    _matrix[source_id][destination_id] = false;
  }

  template<typename T>
  void
  dfs_helper (std::unordered_set<std::string>& visited, T&& vertex) const
  {
    visited.emplace (std::forward<T> (vertex));

    std::cout << std::forward<T> (vertex) << ' ';

    auto id = _key_to_index.at (std::forward<T> (vertex));

    for (u32 i {}; i < _matrix[id].size (); ++i)
      {
	if (_matrix[id][i] == true)
	  {
	    auto edge = _index_to_key.at (i);

	    if (visited.count (edge) == 0)
	      {
		dfs_helper (visited, edge);
	      }
	  }
      }
  }

  void
  dfs () const
  {
    //
    // @NOTE: read comment on dense_undirected_graph.
    //
    std::unordered_set<std::string> visited;

    for (auto const& vertex : _vertices)
      {
	if (visited.count (vertex) == 0)
	  {
	    dfs_helper (visited, vertex);
	  }
      }

    std::cout << '\n';
  }

  void
  bfs () const
  {
    std::unordered_set<std::string> visited;
    std::queue<std::string> nodes;

    for (auto const& vertex : _vertices)
      {
	if (visited.count (vertex) == 0)
	  {
	    visited.emplace (vertex);
	    nodes.emplace (vertex);

	    while (! nodes.empty ())
	      {
		auto current = nodes.front ();
		auto id = _key_to_index.at (current);

		std::cout << current << ' ';

		nodes.pop ();

		for (u32 i {}; i < _matrix[id].size (); ++i)
		  {
		    if (_matrix[id][i] == true)
		      {
			auto edge = _index_to_key.at (i);

			if (visited.count (edge) == 0)
			  {
			    visited.emplace (edge);
			    nodes.emplace (edge);
			  }
		      }
		  }
	      }
	  }
      }

    std::cout << '\n';
  }

  template<typename T>
  bool
  has_edge (T&& source, T&& destination) const
  {
    auto source_it = _key_to_index.find (std::forward<T> (source));
    auto destination_it = _key_to_index.find (std::forward<T> (destination));

    if (source_it == _key_to_index.end () || destination_it == _key_to_index.end ())
      return false;

    auto source_id = source_it->second;
    auto destination_id = destination_it->second;

    return _matrix[source_id][destination_id];
  }

  template<typename T>
  bool
  has_cycle_helper (std::unordered_set<std::string>& visited, std::unordered_set<std::string>& rec_stack, T&& vertex) const
  {
    visited.emplace (std::forward<T> (vertex));
    rec_stack.emplace (std::forward<T> (vertex));

    auto current_vertex_id = _key_to_index.at (std::forward<T> (vertex));

    for (u32 i {}; i < _matrix[current_vertex_id].size (); ++i)
      {
	if (_matrix[current_vertex_id][i] == true)
	  {
	    auto const& edge = _index_to_key.at (i);

	    if (visited.count (edge) == 0)
	      {
		if (has_cycle_helper (visited, rec_stack, edge))
		  {
		    return true;
		  }
	      }
	    else if (rec_stack.count (edge) > 0)
	      {
		return true;
	      }
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
	    if (has_cycle_helper (visited, rec_stack, vertex))
	      {
		return true;
	      }
	  }
      }

    return false;
  }

private:
  // TODO: this member variable might not be useful... the confusion
  // arises because what if the vertex is not just a plain string?
  // you might want to store the data on one side and the keys in
  // another?...
  std::vector<std::string> _vertices;

  std::unordered_map<std::string, u32> _key_to_index;
  std::unordered_map<u32, std::string> _index_to_key;
  std::vector<std::vector<bool>> _matrix;
};


int
main ()
{
  using namespace std::string_literals;

  // Test 1: Empty graph
  {
    std::vector<std::string> vertices;
    dense_directed_graph g (vertices);
    assert (! g.has_cycle ());
    assert (! g.has_edge("A"s, "B"s));
  }

  // Test 2: Single vertex graph
  {
    std::vector<std::string> vertices = {"A"s};
    dense_directed_graph g (vertices);
    assert (! g.has_cycle ());
    assert (! g.has_edge ("A"s, "A"s));
  }

  // Test 3: Two connected components
  {
    std::vector<std::string> vertices = {"A"s, "B"s, "C"s, "D"s};
    dense_directed_graph g (vertices);

    g.add_edge ("A"s, "B"s);
    g.add_edge ("C"s, "D"s);

    assert(! g.has_cycle ());

    // Check edges
    assert (g.has_edge ("A"s, "B"s));
    assert (g.has_edge ("C"s, "D"s));
    assert (! g.has_edge ("A"s, "C"s));
    assert (! g.has_edge ("B"s, "D"s));

    // Add cycle
    g.add_edge ("B"s, "A"s);
    assert (g.has_cycle ());

    // Remove cycle
    g.remove_edge ("B"s, "A"s);
    assert (! g.has_cycle());
  }

  // Test 4: Multiple cycles
  {
    std::vector<std::string> vertices = {"E"s, "F"s, "G"s};
    dense_directed_graph g (vertices);

    g.add_edge ("E"s, "F"s);
    g.add_edge ("F"s, "G"s);
    g.add_edge ("G"s, "E"s);  // Creates a cycle E -> F -> G -> E

    assert (g.has_cycle ());

    g.remove_edge ("E"s, "F"s); // G -> E , F -> G -> E
    assert(! g.has_cycle ());
  }

  // Test 5: Large graph with multiple connected components
  {
    std::vector<std::string> vertices = {"X"s, "Y"s, "Z"s, "W"s, "V"s};
    dense_directed_graph g (vertices);

    g.add_edge ("X"s, "Y"s);
    g.add_edge ("Y"s, "Z"s);
    g.add_edge ("Z"s, "W"s);
    g.add_edge ("W"s, "V"s);
    g.add_edge ("V"s, "X"s);  // Creates a cycle X -> Y -> Z -> W -> V -> X

    assert (g.has_cycle ());

    assert (g.has_edge ("X"s, "Y"s));
    assert (g.has_edge ("Z"s, "W"s));
    assert (g.has_edge ("V"s, "X"s));

    assert (! g.has_edge("X"s, "Z"s));
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
    assert (! g.has_edge ("S"s, "P"s));  // No edge P -> S

    // Remove edge
    g.remove_edge ("P"s, "Q"s);
    assert (! g.has_edge("P"s, "Q"s));
  }

  // Test 7: DFS and BFS traversal
  {
    std::vector<std::string> vertices = {"T"s, "U"s, "V"s, "W"s};
    dense_directed_graph g (vertices);

    g.add_edge ("T"s, "U"s);
    g.add_edge ("U"s, "V"s);
    g.add_edge ("V"s, "W"s);
    g.add_edge ("W"s, "T"s);  // Creates a cycle T -> U -> V -> W -> T

    g.dfs ();
    g.bfs ();
  }

  {
    // Z <- T -> U -> V -> W
    std::vector<std::string> vertices = {"T"s, "U"s, "V"s, "W"s, "Z"s};
    dense_directed_graph g (vertices);

    g.add_edge ("T"s, "Z"s);
    g.add_edge ("T"s, "U"s);
    g.add_edge ("U"s, "V"s);
    g.add_edge ("V"s, "W"s);

    g.dfs ();			// This should be: T U V W Z
    g.bfs ();			// This should be: T U Z V W
  }

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
