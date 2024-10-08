#include <cassert>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <memory>
#include <stack>

using i32 = std::int32_t;
using u32 = std::uint32_t;

// TODO: needs huge cleanup
// assumption: it's a connected graph!!!!!!!
template<typename T>
concept std_string = std::is_same_v<T, std::string>;

struct vertex final
{
  explicit vertex(std_string auto value)
  {
    assert (! value.empty ());
    _value = value;
  }

  std::string _value;
  // more data here if needed...
};

class sparse_directed_graph final
{
public:
  sparse_directed_graph ()
  {}

  ~sparse_directed_graph ()
  {}

  void
  add_vertex (std::unique_ptr<vertex>& v)
  {
    if (_vertices.count (v->_value) > 0)
      return;

    _list[v->_value] = std::unordered_set<std::string> ();
    _vertices[v->_value] = std::move (v);
  }

  void
  remove_vertex (std_string auto key)
  {
    if (key.empty () || empty ())
      return;

    _vertices.erase (key);
    _list.erase (key);

    for (auto& [_, neighbours] : _list)
      neighbours.erase (key);
  }

  void
  add_edge (std_string auto source, std_string auto destination)
  {
    if (source.empty () || destination.empty () || empty () || _vertices.count (source) == 0 || _vertices.count (destination) == 0)
      return;

    _list[source].emplace (destination);
  }

  void
  remove_edge (std_string auto source, std_string auto destination)
  {
    if (source.empty () || destination.empty () || empty () || _vertices.count (source) == 0 || _vertices.count (destination) == 0)
      return;

    _list[source].erase (destination);
  }

  bool
  empty () const
  {
    return _vertices.size () == 0;
  }

  bool
  has_edge (std_string auto source, std_string auto destination) const
  {
    if (empty () || source.empty () || destination.empty () || _vertices.count (source) == 0 || _vertices.count (destination) == 0)
      return false;

    return _list.at (source).count (destination) > 0;
  }

  bool
  has_cycle (std_string auto root) const
  {
    if (root.empty () || empty () || _vertices.count (root) == 0)
      return false;

    std::stack<std::string> current_vertices;
    std::unordered_set<std::string> visited;

    current_vertices.emplace (root);
    visited.emplace (root);

    while (! current_vertices.empty ())
      {
	auto& current_vertex = current_vertices.top ();

	current_vertices.pop ();

	for (auto const& edge: _list.at (current_vertex))
	  {
	    if (visited.count (edge) == 0)
	      {
		current_vertices.emplace (edge);
		visited.emplace (edge);
	      }
	    else
	      {
		return true;
	      }
	  }
      }

    return false;
  }

  void
  print () const
  {

  }

  void
  dfs () const
  {

  }

  void
  bfs () const
  {

  }

private:
  std::unordered_map<std::string, std::unique_ptr<vertex>> _vertices;
  std::unordered_map<std::string, std::unordered_set<std::string>> _list;
};

int
main ()
{
  using namespace std::string_literals;

  sparse_directed_graph graph;

  auto vertex_a = std::make_unique<vertex> ("A"s);
  auto vertex_b = std::make_unique<vertex> ("B"s);
  auto vertex_c = std::make_unique<vertex> ("C"s);
  auto vertex_d = std::make_unique<vertex> ("D"s);

  graph.add_vertex (vertex_a);
  graph.add_vertex (vertex_b);
  graph.add_vertex (vertex_c);
  graph.add_vertex (vertex_d);

  assert (! graph.has_edge ("A"s, "B"s));
  assert (! graph.has_edge ("B"s, "C"s));
  assert (! graph.has_edge ("C"s, "D"s));
  assert (! graph.has_edge ("D"s, "A"s));
  assert (! graph.has_edge ("F"s, "Z"s));

  graph.add_edge ("A"s, "B"s);
  graph.add_edge ("B"s, "C"s);
  graph.add_edge ("C"s, "D"s);

  assert (graph.has_edge ("A"s, "B"s));
  assert (graph.has_edge ("B"s, "C"s));
  assert (graph.has_edge ("C"s, "D"s));

  assert (! graph.has_edge ("B"s, "A"s));
  assert (! graph.has_edge ("C"s, "B"s));
  assert (! graph.has_edge ("D"s, "C"s));

  assert (! graph.has_cycle ("A"s));

  // Add a cycle.
  graph.add_edge ("C"s, "A"s);
  assert (graph.has_cycle ("A"s));

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
