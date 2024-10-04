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

    if (_vertices.find (value) != _vertices.end ()) // no duplicate vertices
      return;

    _vertices[value] = new vertex (value);

    _list[value] = std::vector<std::string> ();
  }

  void
  remove_vertex (std_string auto value)
  {
    if (value.empty ())
      return;

    auto adjacency_it = _list.find (value);

    if (adjacency_it == _list.end ())
      return;

    _list.erase (adjacency_it);

    auto vertex_it = _vertices.find (value);
    delete vertex_it->second;
    _vertices.erase(vertex_it);

    for (auto& [key, adjacency_list] : _list)
      adjacency_list.erase (std::remove (adjacency_list.begin (), adjacency_list.end (), value), adjacency_list.end ());
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

    if (_vertices.find (source)      == _vertices.end () ||
	_vertices.find (destination) == _vertices.end ())
      return;

    // avoid duplicates, add both entries because this is a undirected graph
    if (std::count (_list[source].begin (), _list[source].end (), destination) == 0)
      _list[source].push_back (destination);

    if (std::count (_list[destination].begin (), _list[destination].end (), source) == 0)
      _list[destination].push_back (source);
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

    _list[source_id].erase (std::remove (_list[source_id].begin (), _list[source_id].end (), destination_id), _list[source_id].end ());
    _list[destination_id].erase (std::remove (_list[destination_id].begin (), _list[destination_id].end (), source_id), _list[destination_id].end ());
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

  bool
  empty () const
  {
    return _vertices.size () == 0;
  }

private:
  std::unordered_map<std::string, std::vector<std::string>> _list; // adjacency list, actually
  std::unordered_map<std::string, vertex*> _vertices;
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

  assert (! graph.empty ());

  graph.add_edge ("A"s, "B"s);
  graph.add_edge ("B"s, "C"s);
  graph.add_edge ("C"s, "D"s);

  graph.print ();

  graph.remove_vertex ("D"s);
  graph.remove_vertex ("A"s);

  std::cout << "After removing...\n";

  graph.print ();

  return EXIT_SUCCESS;
}
