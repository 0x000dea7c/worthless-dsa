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

class dense_undirected_graph final
{
public:
  dense_undirected_graph (std::vector<std::string> vertices)
    : _vertices {vertices}
  {
    _matrix.resize (_vertices.size (), std::vector<bool> (vertices.size (), false));

    for (i32 i = 0; i < _vertices.size (); ++i)
      {
	_key_to_index[vertices[i]] = i;
	_index_to_key[i] = vertices[i];
      }
  }

  ~dense_undirected_graph ()
  {
  }

  void
  add_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty ())
      return;

    auto src_it = _key_to_index.find (src);

    if (src_it == _key_to_index.end ())
      return;

    auto dst_it = _key_to_index.find (dst);

    _matrix[src_it->second][dst_it->second] = true;
    _matrix[dst_it->second][src_it->second] = true;
  }

  void
  remove_edge (std::string const& src, std::string const& dst)
  {
    if (src.empty () || dst.empty ())
      return;

    auto src_it = _key_to_index.find (src);

    if (src_it == _key_to_index.end ())
      return;

    auto dst_it = _key_to_index.find (dst);

    _matrix[src_it->second][dst_it->second] = false;
    _matrix[dst_it->second][src_it->second] = false;
  }

  bool
  has_edge (std::string const& src, std::string const& dst) const
  {
    if (src.empty () || dst.empty ())
      return false;

    auto src_it = _key_to_index.find (src);

    if (src_it == _key_to_index.end ())
      return false;

    auto dst_it = _key_to_index.find (dst);

    return _matrix[src_it->second][dst_it->second] && _matrix[dst_it->second][src_it->second];
  }

  bool
  has_cycle_helper (std::string const& vertex, std::string const& parent, std::unordered_set<std::string>& visited) const
  {
    visited.emplace (vertex);

    auto neighbours_id = _key_to_index.at (vertex);

    for (i32 i {0}; i < _matrix[neighbours_id].size (); ++i)
      {
	if (_matrix[neighbours_id][i])
	  {
	    auto edge = _index_to_key.at (i);

	    if (visited.count (edge) == 0)
	      {
		if (has_cycle_helper (edge, vertex, visited))
		  return true;
	      }
	    else if (edge != parent)
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
	      return true;
	  }
      }

    return false;
  }

  void
  dfs_helper (std::string const& vertex, std::unordered_set<std::string>& visited) const
  {
    visited.emplace (vertex);

    std::cout << vertex << ' ';

    auto neighbours_id = _key_to_index.at (vertex);

    for (i32 i {0}; i < _matrix[neighbours_id].size (); ++i)
      {
	if (_matrix[neighbours_id][i])
	  {
	    auto edge = _index_to_key.at (i);

	    if (visited.count (edge) == 0)
	      {
		dfs_helper (edge, visited);
	      }
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
	  dfs_helper (vertex, visited);
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
		auto current_v = current_vertices.front ();

		current_vertices.pop ();

		std::cout << current_v << ' ';

		auto neighbours_id = _key_to_index.at (current_v);

		for (i32 i {0}; i < _matrix[neighbours_id].size (); ++i)
		  {
		    if (_matrix[neighbours_id][i])
		      {
			auto edge = _index_to_key.at (i);

			if (visited.count (edge) == 0)
			  {
			    visited.emplace (edge);
			    current_vertices.emplace (edge);
			  }
		      }
		  }
	      }
	  }
      }

    std::cout << '\n';
  }

private:
  std::vector<std::string> _vertices;
  std::unordered_map<std::string, i32> _key_to_index;
  std::unordered_map<i32, std::string> _index_to_key;
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
