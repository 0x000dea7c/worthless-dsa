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

// TODO: needs cleanup
template<typename T>
concept std_string = std::is_same_v<T, std::string>;

struct vertex final
{
  explicit vertex (std_string auto value)
    : _value {value}
  {}

  std::string _value;
};

// assumption: you won't be adding/removing vertices because it's freaking expensive.
// in fact, you need to pass a vector with every vertex created because fuck it
// assumption: connected unweighted graph
class dense_undirected_graph final
{
public:
  dense_undirected_graph (std::vector<vertex*>& vertices)
  {
    if (vertices.empty ())
      return;

    u32 size = vertices.size ();

    _matrix.resize (size, std::vector<bool> (size, false));

    for (u32 i {}; i < size; ++i)
      {
	auto* v = vertices[i];
	_vertices[v->_value] = v;
	_key_to_index[v->_value] = i;
	_index_to_key[i] = v->_value;
      }
  }

  ~dense_undirected_graph ()
  {
    for (auto& [key, ptr] : _vertices)
      delete ptr;
  }

  void
  add_edge (std_string auto source, std_string auto destination)
  {
    auto source_it = _key_to_index.find (source);
    auto destination_it = _key_to_index.find (destination);

    if (source_it == _key_to_index.end () || destination_it == _key_to_index.end ())
      return;

    auto source_id = source_it->second;
    auto destination_id = destination_it->second;

    _matrix[source_id][destination_id] = true;
    _matrix[destination_id][source_id] = true;
  }

  void
  remove_edge (std_string auto source, std_string auto destination)
  {
    auto source_it = _key_to_index.find (source);
    auto destination_it = _key_to_index.find (destination);

    if (source_it == _key_to_index.end () || destination_it == _key_to_index.end ())
      return;

    auto source_id = source_it->second;
    auto destination_id = destination_it->second;

    // both ways? idk, im stupid
    _matrix[source_id][destination_id] = false;
    _matrix[destination_id][source_id] = false;
  }

  void
  print () const
  {
    for (auto const& [key, data] : _vertices)
      {
	auto index = _key_to_index.at (key);

	for (u32 i {}; i < _matrix[index].size (); ++i)
	  {
	    if (i != index && _matrix[index][i] == true)
	      {
		std::cout << key << ": " << _index_to_key.at (i) <<  '\n';
	      }
	  }
      }
  }

  void
  dfs (std_string auto root) const
  {
    if (_key_to_index.count (root) == 0)
      return;

    std::unordered_set<std::string> visited;
    std::stack<std::string> nodes;
    nodes.push (root);
    visited.emplace (root);

    while (! nodes.empty ())
      {
	auto key = nodes.top ();

	nodes.pop ();

	std::cout << key << '\n';

	auto key_id = _key_to_index.at (key);

	for (u32 i {}; i < _matrix[key_id].size (); ++i)
	  {
	    auto edge_key = _index_to_key.at (i);

	    if (visited.count (edge_key) == 0 && _matrix[key_id][i] == true)
	      {
		visited.emplace (edge_key);
		nodes.push (edge_key);
	      }
	  }
      }
  }

  void
  bfs (std_string auto root) const
  {
    if (_key_to_index.count (root) == 0)
      return;

    std::unordered_set<std::string> visited;
    std::queue<std::string> nodes;
    nodes.push (root);
    visited.emplace (root);

    while (! nodes.empty ())
      {
	auto key = nodes.front ();

	nodes.pop ();

	std::cout << key << '\n';

	auto key_id = _key_to_index.at (key);

	for (u32 i {}; i < _matrix[key_id].size (); ++i)
	  {
	    auto edge_key = _index_to_key.at (i);

	    if (visited.count (edge_key) == 0 && _matrix[key_id][i] == true)
	      {
		visited.emplace (edge_key);
		nodes.push (edge_key);
	      }
	  }
      }
  }

  bool
  has_edge (std_string auto source, std_string auto destination) const
  {
    auto source_it = _key_to_index.find (source);
    auto destination_it = _key_to_index.find (destination);

    if (source_it == _key_to_index.end () || destination_it == _key_to_index.end ())
      return false;

    auto source_id = source_it->second;
    auto destination_id = destination_it->second;

    return _matrix[source_id][destination_id];
  }

private:
  std::unordered_map<std::string, vertex*> _vertices; // don't know about this, i think this is stupid
  std::unordered_map<std::string, u32> _key_to_index;
  std::unordered_map<u32, std::string> _index_to_key;
  std::vector<std::vector<bool>> _matrix;
};

int
main ()
{
  using namespace std::string_literals;

  std::vector<vertex*> vertices;
  vertices.push_back (new vertex ("A"s));
  vertices.push_back (new vertex ("B"s));
  vertices.push_back (new vertex ("C"s));
  vertices.push_back (new vertex ("D"s));

  dense_undirected_graph graph (vertices);

  graph.add_edge ("A"s, "B"s);
  graph.add_edge ("B"s, "C"s);
  graph.add_edge ("C"s, "D"s);

  assert (graph.has_edge ("A"s, "B"s));
  assert (graph.has_edge ("B"s, "A"s));

  assert (graph.has_edge ("B"s, "C"s));
  assert (graph.has_edge ("C"s, "B"s));

  assert (graph.has_edge ("C"s, "D"s));
  assert (graph.has_edge ("D"s, "C"s));

  assert (graph.has_edge ("C"s, "D"s));
  assert (graph.has_edge ("D"s, "C"s));

  std::cout << "...Printing all graph...\n";
  graph.print ();

  std::cout << "...dfs...\n";
  graph.dfs ("B"s);

  std::cout << "...bfs...\n";
  graph.bfs ("B"s);

  graph.remove_edge ("A"s, "B"s);

  assert (! graph.has_edge ("A"s, "B"s));

  // disconnected graph
  std::vector<vertex*> vertices2 = {new vertex ("X"s), new vertex ("Y"s), new vertex ("Z"s)};
  dense_undirected_graph graph2 (vertices2);
  assert (! graph2.has_edge ("X"s, "Y"s));
  assert (! graph2.has_edge ("Y"s, "Z"s));

  // graph with self edge
  std::vector<vertex*> vertices3 = {new vertex ("P"s), new vertex ("Q"s), new vertex ("R"s)};
  dense_undirected_graph graph3 (vertices3);
  graph3.add_edge ("P"s, "Q"s);
  graph3.add_edge ("P"s, "R"s);
  graph3.add_edge ("Q"s, "R"s);
  assert(graph3.has_edge("P"s, "Q"s));
  assert(graph3.has_edge("P"s, "R"s));
  assert(graph3.has_edge("Q"s, "R"s));
  graph3.add_edge("P"s, "P"s);
  assert(graph3.has_edge("P"s, "P"s));

  // this dfs shouldn't show a self-edge (but it's good if you ask)
  std::cout << "...Printing dfs of graph that has a self edge...\n";
  graph3.dfs ("P"s);

  // empty graph
  std::vector<vertex*> vertices4;
  dense_undirected_graph graph4 (vertices4);
  assert (! graph4.has_edge ("A"s, "B"s)); // don't crash bitch

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
