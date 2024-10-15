#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <queue>

using i32 = std::int32_t;
using u32 = std::uint32_t;

class dense_undirected_graph final
{
public:
  dense_undirected_graph (std::vector<std::string>& vertices)
  {
    _matrix.resize (vertices.size (), std::vector<bool> (vertices.size (), false));

    for (u32 i {}; i < vertices.size (); ++i)
      {
	_index_to_key[i] = vertices[i];
	_key_to_index[vertices[i]] = i;
      }
  }

  ~dense_undirected_graph () = default;

  void
  add_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty () ||
	_key_to_index.count (src) == 0 ||
	_key_to_index.count (dst) == 0)
      return;

    auto src_id = _key_to_index.at (src);
    auto dst_id = _key_to_index.at (dst);

    _matrix[src_id][dst_id] = true;
    _matrix[dst_id][src_id] = true;
  }

  void
  remove_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty () ||
	_key_to_index.count (src) == 0 ||
	_key_to_index.count (dst) == 0)
      return;

    auto src_id = _key_to_index.at (src);
    auto dst_id = _key_to_index.at (dst);

    _matrix[src_id][dst_id] = false;
    _matrix[dst_id][src_id] = false;
  }

  bool
  has_edge (std::string const& src, std::string const& dst) const
  {
    if (src.empty () || dst.empty () ||
	_key_to_index.count (src) == 0 ||
	_key_to_index.count (dst) == 0)
      return false;

    auto src_id = _key_to_index.at (src);
    auto dst_id = _key_to_index.at (dst);

    return _matrix[src_id][dst_id];
  }

  bool
  has_cycle_helper (u32 vertex, u32 parent, std::unordered_set<u32>& visited) const
  {
    visited.emplace (vertex);

    for (u32 i {}; i < _matrix[vertex].size (); ++i)
      {
	if (_matrix[vertex][i] && visited.count (i) == 0)
	  {
	    if (has_cycle_helper (i, vertex, visited))
	      return true;
	  }
	else if (_matrix[vertex][i] && i != parent)
	  return true;
      }

    return false;
  }

  bool
  has_cycle () const
  {
    std::unordered_set<u32> visited;

    for (u32 i {}; i < _matrix.size (); ++i)
      {
	if (visited.count (i) == 0)
	  {
	    if (has_cycle_helper (i, std::numeric_limits<u32>::max (), visited))
	      return true;
	  }
      }

    return false;
  }

  void
  dfs_helper (u32 vertex, std::unordered_set<u32>& visited) const
  {
    visited.emplace (vertex);

    std::cout << _index_to_key.at (vertex) << ' ';

    for (u32 i {}; i < _matrix[vertex].size (); ++i)
      {
	if (_matrix[vertex][i] && visited.count (i) == 0)
	  {
	    dfs_helper (i, visited);
	  }
      }
  }

  void
  dfs () const
  {
    std::unordered_set<u32> visited;

    for (u32 i {}; i < _matrix.size (); ++i)
      {
	if (visited.count (i) == 0)
	  {
	    dfs_helper (i, visited);
	  }
      }

    std::cout << '\n';
  }

  void
  bfs () const
  {
    std::unordered_set<u32> visited;
    std::queue<u32> current_vertices;

    for (u32 i {}; i < _matrix.size (); ++i)
      {
	if (visited.count (i) == 0)
	  {
	    visited.emplace (i);
	    current_vertices.emplace (i);

	    while (! current_vertices.empty ())
	      {
		auto current_vertex = current_vertices.front ();

		current_vertices.pop ();

		std::cout << _index_to_key.at (current_vertex) << ' ';

		for (u32 j {}; j < _matrix[current_vertex].size (); ++j)
		  {
		    if (_matrix[current_vertex][j] && visited.count (j) == 0)
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

private:
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

  assert (! graph.has_cycle ());

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
  assert (! graph2.has_cycle ());

  // graph with self edge
  std::vector<std::string> vertices3 = {"P"s, "Q"s, "R"s, "T"s};
  dense_undirected_graph graph3 (vertices3);
  graph3.add_edge ("P"s, "Q"s);
  graph3.add_edge ("P"s, "R"s);
  graph3.add_edge ("Q"s, "R"s);
  assert (graph3.has_cycle ());

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
  assert (! graph4.has_cycle ());

  std::vector<std::string> vertices5 {
    "A"s, "B"s, "C"s, "D"s
  };

  //
  // some more misc testing...
  //
  dense_undirected_graph graph5 (vertices5);

  graph5.add_edge ("A"s, "B"s);
  graph5.add_edge ("A"s, "C"s);
  graph5.add_edge ("B"s, "D"s);
  assert (! graph5.has_cycle ());

  std::cout << "...Graph 5 dfs...\n";
  graph5.dfs ();

  std::cout << "...Graph 5 bfs...\n";
  graph5.bfs ();

  std::vector<std::string> vertices6 {"A"s, "B"s, "C"s};
  dense_undirected_graph graph6 (vertices6);

  graph6.add_edge ("A"s, "B"s);
  graph6.add_edge ("B"s, "C"s);
  graph6.add_edge ("C"s, "A"s);

  assert (graph6.has_cycle ());

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
