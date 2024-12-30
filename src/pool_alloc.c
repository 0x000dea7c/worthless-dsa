#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_ALIGNMENT (2 * sizeof(void*))

typedef struct pool_free_node pool_free_node;
typedef struct arena_pool arena_pool;

struct pool_free_node {
	pool_free_node *next;
};

struct arena_pool {
	unsigned char *buf;
	size_t buf_len;
	size_t chunk_size;
	pool_free_node *head;
};

static bool ispowof2(uintptr_t x) {
	return (x & (x - 1)) == 0;
}

static uintptr_t align_forward_uintptr(uintptr_t start, uintptr_t align)
{
	uintptr_t mod = start & (align - 1);
	if (mod != 0) {
		start += align - mod;
	}
	return start;
}

static size_t align_forward_size(size_t chunk_size, size_t align)
{
	size_t mod = chunk_size & (align - 1);
	if (mod != 0) {
		chunk_size += align - mod;
	}
	return chunk_size;
}

void arena_pool_free_all(arena_pool *a)
{
	size_t chunk_count = a->buf_len / a->chunk_size;

	for (size_t i = 0; i < chunk_count; ++i) {
		void *ptr = &a->buf[i * a->chunk_size];
		pool_free_node *node = (pool_free_node *) ptr;
		node->next = a->head;
		a->head = node;
	}
}

void *arena_pool_alloc(arena_pool *a)
{
	pool_free_node *next = a->head;

	if (!next) {
		assert(false && "not enough mem available");
		return next;
	}

	a->head = a->head->next;

	return memset(next, 0, a->chunk_size);
}

void arena_pool_free(arena_pool *a, void *ptr)
{
	if (!ptr) {
		return;
	}

	void *start = a->buf;
	void *end = &a->buf[a->buf_len];

	if (!(ptr >= start && ptr < end)) {
		assert(false && "ptr does not belong to this arena");
		return;
	}

	pool_free_node *node = (pool_free_node *) ptr;
	node->next = a->head;
	a->head = node;
}

void arena_pool_init_align(arena_pool *a, void *buf, size_t buf_len, size_t chunk_size, size_t chunk_align)
{
	assert(ispowof2(chunk_align));
	uintptr_t initial_start = (uintptr_t) buf;
	uintptr_t aligned_start = align_forward_uintptr(initial_start, (uintptr_t) chunk_align);
	buf_len -= (size_t) (aligned_start - initial_start);

	chunk_size = align_forward_size(chunk_size, chunk_align);

	assert(chunk_size >= sizeof(pool_free_node) && "chunk size is too small");
	assert(buf_len >= chunk_size && "buffer length is smaller than the chunk size");

	a->buf = buf;
	a->buf_len = buf_len;
	a->chunk_size = chunk_size;
	a->head = NULL;
	arena_pool_free_all(a);
}

void arena_pool_init(arena_pool *a, void *buf, size_t buf_len, size_t chunk_size)
{
	arena_pool_init_align(a, buf, buf_len, chunk_size, DEFAULT_ALIGNMENT);
}

int main(void) {
	size_t chunk_size = 32;
	size_t buf_len = 1024;
	unsigned char *buf = malloc(buf_len);

	arena_pool arena;
	arena_pool_init(&arena, buf, buf_len, chunk_size);

        int *numbers = arena_pool_alloc(&arena);
	for (int i = 0; i < 8; ++i)
		numbers[i] = i + 1;

	for (int i = 0; i < 8; ++i)
		printf("%d\n", numbers[i]);

	int *more_numbers = arena_pool_alloc(&arena);
	for (int i = 0; i < 8; ++i)
		more_numbers[i] = 7 + i + 1;

	for (int i = 0; i < 8; ++i)
		printf("%d\n", more_numbers[i]);

	char *dangerous = arena_pool_alloc(&arena);
	memcpy(dangerous, "Lain", sizeof("Lain"));
	memcpy(dangerous + sizeof("Lain") + 1, "Julia", sizeof("Julia"));

	printf("%s - %s\n", dangerous, &dangerous[6]);

	free(buf);

	return 0;
}
