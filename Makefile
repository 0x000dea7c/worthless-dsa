binary_min_heap:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined binary_min_heap.cc -o binary_min_heap

queue:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined queue.cc -o queue

binary_search_tree:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined binary_search_tree.cc -o binary_search_tree

trie:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined trie.cc -o trie

singly_linked_list:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined singly_linked_list.cc -o singly_linked_list

doubly_linked_list:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined doubly_linked_list.cc -o doubly_linked_list

dynamic_array:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined dynamic_array.cc -o dynamic_array

sparse_undirected_graph:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined sparse_undirected_graph.cc -o sparse_undirected_graph

dense_undirected_graph:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined dense_undirected_graph.cc -o dense_undirected_graph

string_permutation:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined string_permutation.cc -o string_permutation

sparse_directed_graph:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined sparse_directed_graph.cc -o sparse_directed_graph

dense_directed_graph:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined dense_directed_graph.cc -o dense_directed_graph

.PHONY: binary_search_tree sparse_undirected_graph   \
	dense_undirected_graph doubly_linked_list    \
	string_permutation sparse_directed_graph     \
	trie singly_linked_list dense_directed_graph \
	binary_min_heap \
	queue
