all:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined binary_min_heap.cc -o binary_min_heap
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined queue.cc -o queue
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined binary_search_tree.cc -o binary_search_tree
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined trie.cc -o trie
