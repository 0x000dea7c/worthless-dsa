#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <queue>
#include <limits>

class dense_undirected_graph final
{
public:
  dense_undirected_graph (std::vector<std::string> &vertices) : _index{0}
  {
    _matrix.resize (vertices.size (), std::vector<size_t> (vertices.size (), 0));

    for (auto const &vertex : vertices)
      {
        _key_to_index[vertex] = _index;
        _index_to_key[_index] = vertex;
        ++_index;
      }
  }

  ~dense_undirected_graph () = default;

  bool add_edge (std::string const &src, std::string const &dst)
  {
    assert (!src.empty ());
    assert (!dst.empty ());

    if (_key_to_index.count (src) == 0 || _key_to_index.count (dst) == 0)
      {
        return false;
      }

    auto src_index = _key_to_index[src];
    auto dst_index = _key_to_index[dst];

    _matrix[src_index][dst_index] = 1;
    _matrix[dst_index][src_index] = 1;

    return true;
  }

  bool remove_edge (std::string const &src, std::string const &dst)
  {
    assert (!src.empty ());
    assert (!dst.empty ());

    if (_key_to_index.count (src) == 0 || _key_to_index.count (dst) == 0)
      {
        return false;
      }

    auto src_index = _key_to_index[src];
    auto dst_index = _key_to_index[dst];

    _matrix[src_index][dst_index] = 0;
    _matrix[dst_index][src_index] = 0;

    return true;
  }

  bool has_edge (std::string const &src, std::string const &dst) const
  {
    assert (!src.empty ());
    assert (!dst.empty ());

    if (_key_to_index.count (src) == 0 || _key_to_index.count (dst) == 0)
      {
        return false;
      }

    auto src_index = _key_to_index.at (src);
    auto dst_index = _key_to_index.at (dst);

    return _matrix[src_index][dst_index] == 1;
  }

  void dfs_helper (size_t vertex, std::set<size_t> &visited) const
  {
    visited.emplace (vertex);

    std::cout << _index_to_key.at (vertex) << ' ';

    for (size_t i = 0; i < _matrix[vertex].size (); ++i)
      {
        if (visited.count (i) == 0 && _matrix[vertex][i] != 0)
          {
            dfs_helper (i, visited);
          }
      }
  }

  void dfs () const
  {
    std::set<size_t> visited;

    for (size_t i = 0; i < _matrix.size (); ++i)
      {
        for (size_t j = 0; j < _matrix.size (); ++j)
          {
            if (visited.count (j) == 0)
              {
                dfs_helper (j, visited);
              }
          }
      }

    std::cout << '\n';
  }

  void bfs () const
  {
    std::queue<size_t> current_vertices;
    std::set<size_t> visited;

    for (auto const &row : _matrix)
      {
        for (auto const &index : row)
          {
            if (visited.count (index) == 0)
              {
                visited.emplace (index);
                current_vertices.emplace (index);

                while (!current_vertices.empty ())
                  {
                    auto curr = current_vertices.front ();

                    current_vertices.pop ();

                    std::cout << _index_to_key.at (curr) << ' ';

                    for (size_t i = 0; i < _matrix[curr].size (); ++i)
                      {
                        if (visited.count (i) == 0 && _matrix[curr][i] != 0)
                          {
                            visited.emplace (i);
                            current_vertices.emplace (i);
                          }
                      }
                  }
              }
          }
      }

    std::cout << '\n';
  }

  bool has_cycle_helper (size_t vertex, size_t parent, std::set<size_t> &visited) const
  {
    visited.emplace (vertex);

    for (size_t i = 0; i < _matrix.size (); ++i)
      {
        if (visited.count (i) == 0 && _matrix[vertex][i] != 0)
          {
            if (has_cycle_helper (i, vertex, visited))
              {
                return true;
              }
          }
        else if (_matrix[vertex][i] != 0 && i != parent)
          {
            return true;
          }
      }

    return false;
  }

  bool has_cycle () const
  {
    std::set<size_t> visited;

    for (size_t i = 0; i < _matrix.size (); ++i)
      {
        for (size_t j = 0; j < _matrix.size (); ++j)
          {
            if (visited.count (j) == 0)
              {
                if (has_cycle_helper (j, std::numeric_limits<size_t>::max (), visited))
                  {
                    return true;
                  }
              }
          }
      }

    return false;
  }

private:
  std::map<std::string, size_t> _key_to_index;
  std::map<size_t, std::string> _index_to_key;
  std::vector<std::vector<size_t>> _matrix;
  size_t _index;
};

int
main ()
{
  using namespace std::string_literals;

  std::vector<std::string> vertices{"0"s, "1"s, "2"s, "3"s, "4"s, "5"s, "6"s, "7"s};

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

  assert (!graph.has_cycle ());

  std::cout << "...Printing graph...\n";

  std::cout << "...dfs...\n";
  graph.dfs ();

  std::cout << "...bfs...\n";
  graph.bfs ();

  graph.remove_edge ("A"s, "B"s);

  assert (!graph.has_edge ("A"s, "B"s));

  // disconnected graph
  std::vector<std::string> vertices2 = {"X"s, "Y"s, "Z"s};
  dense_undirected_graph graph2 (vertices2);
  assert (!graph2.has_edge ("X"s, "Y"s));
  assert (!graph2.has_edge ("Y"s, "Z"s));
  assert (!graph2.has_cycle ());

  // graph with self edge
  std::vector<std::string> vertices3 = {"P"s, "Q"s, "R"s, "T"s};
  dense_undirected_graph graph3 (vertices3);
  graph3.add_edge ("P"s, "Q"s);
  graph3.add_edge ("P"s, "R"s);
  graph3.add_edge ("Q"s, "R"s);
  assert (graph3.has_cycle ());

  assert (graph3.has_edge ("P"s, "Q"s));
  assert (graph3.has_edge ("P"s, "R"s));
  assert (graph3.has_edge ("Q"s, "R"s));

  graph3.add_edge ("P"s, "P"s);
  assert (graph3.has_edge ("P"s, "P"s));

  // this dfs shouldn't show a self-edge (but it's good if you ask)
  std::cout << "...Printing dfs of graph that has a self edge...\n";
  graph3.dfs ();
  std::cout << "...Printing BFS...\n";
  graph3.bfs ();

  // empty graph
  std::vector<std::string> vertices4;
  dense_undirected_graph graph4 (vertices4);
  assert (!graph4.has_edge ("A"s, "B"s)); // don't crash bitch
  assert (!graph4.has_cycle ());

  std::vector<std::string> vertices5{"A"s, "B"s, "C"s, "D"s};

  //
  // some more misc testing...
  //
  dense_undirected_graph graph5 (vertices5);

  graph5.add_edge ("A"s, "B"s);
  graph5.add_edge ("A"s, "C"s);
  graph5.add_edge ("B"s, "D"s);
  assert (!graph5.has_cycle ());

  std::cout << "...Graph 5 dfs...\n";
  graph5.dfs ();

  std::cout << "...Graph 5 bfs...\n";
  graph5.bfs ();

  std::vector<std::string> vertices6{"A"s, "B"s, "C"s};
  dense_undirected_graph graph6 (vertices6);

  graph6.add_edge ("A"s, "B"s);
  graph6.add_edge ("B"s, "C"s);
  graph6.add_edge ("C"s, "A"s);

  assert (graph6.has_cycle ());

  std::cout << "All tests passed!\n";

  return EXIT_SUCCESS;
}
