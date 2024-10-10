#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <stack>
#include <queue>

using i32 = std::int32_t;
using u32 = std::uint32_t;

// assumption: you won't be adding/removing vertices because it's freaking expensive.
// in fact, you need to pass a vector with every vertex created because fuck it
class dense_undirected_graph final
{
public:
  dense_undirected_graph (std::vector<std::string>& vertices)
  {
    if (vertices.empty ())
      return;

    u32 size = vertices.size ();

    _matrix.resize (size, std::vector<bool> (size, false));

    for (u32 i {}; i < size; ++i)
      {
	_vertices.emplace_back (vertices[i]);
	_key_to_index[vertices[i]] = i;
	_index_to_key[i] = vertices[i];
      }
  }

  ~dense_undirected_graph ()
  {
  }

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
    _matrix[destination_id][source_id] = true;
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
    _matrix[destination_id][source_id] = false;
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
    // Don't know about this. Many implementations of DFS have an
    // argument that indicates the starting point, but that will only
    // work if the graph is connected. If it's not, DFS won't visit
    // every vertex.
    //
    // Then, surprisingly enough, when investigating implementations
    // of functions that check if the graph has a cycle, all of sudden
    // they do take into account that the graph could be disconnected
    // or not.
    //
    // I don't fucking understand. Why? They don't even mention it. So
    // fucking frustrating.
    //
    // I also hate recursion, but it's simpler to implement it this
    // way. Otherwise the implementation would be too complicated for
    // an interview... I assume...
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

  std::vector<std::string> vertices {
    "0"s, "1"s, "2"s, "3"s, "4"s, "5"s, "6"s, "7"s
  };

  dense_undirected_graph graph (vertices);

  graph.add_edge ("0"s, "1"s);
  graph.add_edge ("0"s, "2"s);
  graph.add_edge ("1"s, "3"s);
  graph.add_edge ("1"s, "4"s);
  graph.add_edge ("2"s, "5"s);
  graph.add_edge ("2"s, "6"s);
  graph.add_edge ("3"s, "7"s);

  assert (graph.has_edge ("0"s, "1"s));
  assert (graph.has_edge ("1"s, "0"s));

  assert (graph.has_edge ("1"s, "3"s));
  assert (graph.has_edge ("3"s, "1"s));

  assert (graph.has_edge ("2"s, "6"s));
  assert (graph.has_edge ("6"s, "2"s));

  assert (graph.has_edge ("1"s, "4"s));
  assert (graph.has_edge ("4"s, "1"s));

  std::cout << "...Printing graph...\n";

  std::cout << "...dfs...\n";
  graph.dfs ();

  std::cout << "...bfs...\n";
  graph.bfs ();

  graph.remove_edge ("A"s, "B"s);

  assert (! graph.has_edge ("A"s, "B"s));

  // disconnected graph
  std::vector<std::string> vertices2 = {"X"s, "Y"s, "Z"s};
  dense_undirected_graph graph2 (vertices2);
  assert (! graph2.has_edge ("X"s, "Y"s));
  assert (! graph2.has_edge ("Y"s, "Z"s));

  // graph with self edge
  std::vector<std::string> vertices3 = {"P"s, "Q"s, "R"s, "T"s};
  dense_undirected_graph graph3 (vertices3);
  graph3.add_edge ("P"s, "Q"s);
  graph3.add_edge ("P"s, "R"s);
  graph3.add_edge ("Q"s, "R"s);

  assert (graph3.has_edge("P"s, "Q"s));
  assert (graph3.has_edge("P"s, "R"s));
  assert (graph3.has_edge("Q"s, "R"s));

  graph3.add_edge ("P"s, "P"s);
  assert (graph3.has_edge("P"s, "P"s));

  // this dfs shouldn't show a self-edge (but it's good if you ask)
  std::cout << "...Printing dfs of graph that has a self edge...\n";
  graph3.dfs ();
  std::cout << "...Printing BFS...\n";
  graph3.bfs ();

  // empty graph
  std::vector<std::string> vertices4;
  dense_undirected_graph graph4 (vertices4);
  assert (! graph4.has_edge ("A"s, "B"s)); // don't crash bitch

  std::vector<std::string> vertices5 {
    "A"s, "B"s, "C"s, "D"s
  };

  dense_undirected_graph graph5 (vertices5);

  graph5.add_edge ("A"s, "B"s);
  graph5.add_edge ("A"s, "C"s);
  graph5.add_edge ("B"s, "D"s);

  std::cout << "...Graph 5 dfs...\n";
  graph5.dfs ();

  std::cout << "...Graph 5 bfs...\n";
  graph5.bfs ();

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
