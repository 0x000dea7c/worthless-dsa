#include <cassert>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <iostream>

using i32 = std::int32_t;
using u32 = std::uint32_t;

template<typename T>
concept std_string = std::is_same_v<T, std::string>;

struct vertex final
{
  vertex (std_string auto value)
    : _value {value}
  {
  }

  std::string _value;
};

class sparse_undirected_graph final
{
public:
  sparse_undirected_graph ()
  {

  }

  ~sparse_undirected_graph ()
  {
    for (auto p : _vertices)
      delete p.second;
  }

  void
  add_vertex (std_string auto value)
  {
    if (value.empty ())
      return;

    auto id = std::hash<std::string>{} (value);

    if (_vertices.find (id) != _vertices.end ()) // no duplicate vertices
      return;

    _vertices[id] = new vertex (value);

    _list[id] = std::vector<u32> ();
  }

  void
  remove_vertex (std_string auto value)
  {
    if (value.empty ())
      return;

    auto id = std::hash<std::string>{} (value);

    auto it = _list.find (id);

    if (it == _list.end ())
      return;

    _list.erase (it);

    delete _vertices[id];

    _vertices.erase (_vertices.find (id));
  }

  void
  add_edge (std_string auto source, std_string auto destination)
  {
    // preconditions:
    //
    // 1.- source and destination are non-empty strings
    // 2.- they were already added using add_vertex
    //
    if (source.empty () || destination.empty ())
      return;

    auto source_id      = std::hash<std::string>{} (source);
    auto destination_id = std::hash<std::string>{} (destination);

    if (_vertices.find (source_id)      == _vertices.end () ||
	_vertices.find (destination_id) == _vertices.end ())
      return;

    // avoid duplicates, add both entries because this is a undirected graph
    if (std::count (_list[source_id].begin (), _list[source_id].end (), destination_id) == 0)
      _list[source_id].push_back (destination_id);

    if (std::count (_list[destination_id].begin (), _list[destination_id].end (), source_id) == 0)
      _list[destination_id].push_back (source_id);
  }

  void
  remove_edge (std_string auto source, std_string auto destination)
  {
    // preconditions:
    //
    // 1.- source and destination are non-empty strings
    // 2.- they were already added using add_edge
    //
    if (source.empty () || destination.empty ())
      return;

    auto source_id      = std::hash<std::string>{} (source);
    auto destination_id = std::hash<std::string>{} (destination);

    if (_vertices.find (source_id)      == _vertices.end () ||
	_vertices.find (destination_id) == _vertices.end ())
      return;

    std::remove_if (_list[source_id].begin (), _list[source_id].end (), [destination_id] (u32 edge) { return edge == destination_id; });
    std::remove_if (_list[destination_id].begin (), _list[destination_id].end (), [source_id] (u32 edge) { return edge == source_id; });
  }

  void
  print () const
  {
    for (auto const& vertex : _vertices)
      {
	std::cout << vertex.second->_value << ": ";
	for (auto const& edge : _list.at (vertex.first))
	  {
	    std::cout << _vertices.at (edge)->_value << ' ';
	  }
	std::cout << '\n';
      }
  }

  u32
  size () const
  {
    return _vertices.size ();
  }

  bool
  empty () const
  {
    return size () == 0;
  }

private:
  std::unordered_map<u32, std::vector<u32>> _list; // adjacency list, actually
  std::unordered_map<u32, vertex*> _vertices;
};

int
main ()
{
  using namespace std::string_literals;

  sparse_undirected_graph graph;

  graph.add_vertex ("A"s);
  graph.add_vertex ("B"s);
  graph.add_vertex ("C"s);
  graph.add_vertex ("D"s);

  graph.add_edge ("A"s, "B"s);
  graph.add_edge ("B"s, "C"s);
  graph.add_edge ("C"s, "D"s);

  graph.print ();

  return EXIT_SUCCESS;
}
