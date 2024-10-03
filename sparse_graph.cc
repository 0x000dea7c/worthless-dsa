#include <cassert>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <utility>
#include <iostream>

using i32 = std::int32_t;
using u32 = std::uint32_t;

// assumption: it's an undirected sparse graph
class sparse_graph final
{
public:
  sparse_graph ()
    : _vertex_index   {},
      _adjacency_list {},
      _current_index  {0}
  {}

  void
  add_edge (std::pair<std::string, std::string> const& edge)
  {
    // assumption: if you pass a vertex that doesn't exist, you need to create it?
    assign_indexes_to_vertices (edge);
    update_adjacency_list (edge);
  }

  void
  print_adjacency_list () const
  {
    for (auto const& pair : _vertex_index)
      {
	std::cout << pair.first << "-> ";
	for (auto const& neighbour_index : _adjacency_list[pair.second])
	  {
	    for (auto const& reverse_pair : _vertex_index) // this is to get the string representation of the neighbour
	      {
		if (reverse_pair.second == neighbour_index) // pretty bad
		  {
		    std::cout << reverse_pair.first << " ";
		    break;
		  }
	      }
	  }
	std::cout << '\n';
      }
    std::cout << '\n';
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
  void
  assign_indexes_to_vertices (std::pair<std::string, std::string> const& edge)
  {

    if (_vertex_index.find (edge.first) == _vertex_index.end ())
      {
	_vertex_index[edge.first] = _current_index++;
	_adjacency_list.resize (_current_index);
      }

    if (_vertex_index.find (edge.second) == _vertex_index.end ())
      {
	_vertex_index[edge.second] = _current_index++;
	_adjacency_list.resize (_current_index);
      }
  }

  void
  update_adjacency_list (std::pair<std::string, std::string> const& edge)
  {
    u32 source      {_vertex_index[edge.first]};
    u32 destination {_vertex_index[edge.second]};
    // undirected
    _adjacency_list[source].emplace_back (destination);
    _adjacency_list[destination].emplace_back (source);
  }

  std::unordered_map<std::string, u32> _vertex_index;
  std::vector<std::vector<u32>> _adjacency_list;
  u32 _current_index;
};

int
main ()
{
  sparse_graph graph;

  graph.add_edge (std::make_pair<> ("A", "B"));
  graph.add_edge (std::make_pair<> ("B", "C"));
  graph.add_edge (std::make_pair<> ("C", "D"));
  graph.add_edge (std::make_pair<> ("F", "G"));

  graph.print_adjacency_list ();

  return EXIT_SUCCESS;
}
