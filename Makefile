all:
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined binary_min_heap.cc -o binary_min_heap
	g++ -std=gnu++23 -ggdb -fsanitize=address,leak,undefined queue.cc -o queue
