#include <cstdlib>
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using i32 = std::int32_t;
using u32 = std::uint32_t;

template<typename T>
concept std_string = std::is_same_v<T, std::string>;

struct vertex final
{
  vertex (std_string auto value)
    : _value {value}
  {}

  std::string _value;
};

// also unweighted graph!!!!!!
// assumption: you won't be adding/removing vertices because it's freaking expensive.
// in fact, you need to pass a vector with every vertex created because fuck it
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

  }

  void
  bfs (std_string auto root) const
  {

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

  graph.print ();

  return EXIT_SUCCESS;
}
