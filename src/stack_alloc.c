#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define DEFAULT_ALIGNMENT (2 * sizeof(void*))

typedef struct stack_arena stack_arena;
typedef struct stack_alloc_header stack_alloc_header;

/* Small stack allocator */
struct stack_arena {
	unsigned char *buf;
	size_t buf_len;
	size_t offset;
};

struct stack_alloc_header {
	uint8_t padding;	/* Stores the amount of bytes that has to be placed before the header in order to have the new allocation correctly aligned */
};

static bool ispowof2(uintptr_t x) {
	return (x & (x - 1)) == 0;
}

static size_t compute_padding_with_header(uintptr_t ptr, uintptr_t align, uintptr_t header_size) {
	assert(ispowof2(align) && "alignment isn't a power of 2");
	uintptr_t p = ptr, a = align, padding = 0, needed_space = (uintptr_t) header_size;
	uintptr_t const mod = p & (a - 1); /* p % a, but faster. Assume a is a power of 2 */

	if (mod != 0) {
		padding = a - mod;
	}

	if (padding < needed_space) {
		needed_space -= padding;

		if ((needed_space & (a - 1)) != 0) {
			padding += a * (1 + (needed_space / a));
		} else {
			padding += a * (needed_space / a);
		}
	}

	return (size_t) padding;
}

static void *arena_stack_alloc_align(stack_arena *a, size_t size, size_t align) {
	assert(ispowof2(align) && "alignment isn't a power of 2");
	assert(align <= 128 && "alignment cannot exceed 128!");
	stack_alloc_header *header;
        uintptr_t const curr_addr = (uintptr_t) a->buf + (uintptr_t) a->offset;
	size_t const padding = compute_padding_with_header(curr_addr, (uintptr_t) align, sizeof(stack_alloc_header));

	if (a->offset + padding + size > a->buf_len) {
		fprintf(stderr, "There isn't enough space in the arena");
		return NULL;
	}

	/* Padding done */
	a->offset += padding;

	/* Now header placement */
	uintptr_t next_addr = curr_addr + (uintptr_t) padding; /* where our aligned memory will start (already considers the header) */
	header = (stack_alloc_header *) (next_addr - sizeof(*header)); /* place the header just before the aligned memory */
	header->padding = (uint8_t) padding;

	a->offset += size;

	return memset((void *) next_addr, 0, size);
}

void arena_stack_init(stack_arena *a, void *buf, size_t buf_len) {
	a->buf = (unsigned char *) buf;
	a->buf_len = buf_len;
	a->offset = 0;
}

void *arena_stack_alloc(stack_arena *a, size_t size) {
	return arena_stack_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

void arena_stack_free(stack_arena *a, void* ptr) {
	/* This doesn't actually enforce the LIFO principle, but whatever */
	if (!ptr) {
		return;
	}

	stack_alloc_header const *header;
	uintptr_t const start = (uintptr_t) a->buf;
	uintptr_t const end = start + (uintptr_t) a->buf_len;
	uintptr_t const curr = (uintptr_t) ptr;

	if (!(start <= curr && end > curr)) {
		assert(false && "This pointer doesn't belong to this arena!");
		return;
	}

	if (curr >= start + (uintptr_t) a->offset) {
		/* This is to say that I'm passing a pointer that was already freed, so I don't do anything. */
		return;
	}

	header = (stack_alloc_header *) (curr - sizeof(*header));
	size_t prev_offset = (size_t) (curr - header->padding - start);
	a->offset = prev_offset;
	/* TODO: do I want to set ptr to NULL in here? */
}

void *arena_stack_resize_align(stack_arena *a, void *ptr, size_t old_size, size_t new_size, size_t align) {
	if (ptr == NULL) {
		return arena_stack_alloc_align(a, new_size, align);
	} else if (new_size == 0) {
		arena_stack_free(a, ptr);
		return NULL;
	} else {
		size_t const min_size = new_size < old_size ? new_size : old_size;
		uintptr_t const start = (uintptr_t) a->buf;
		uintptr_t const end = start + a->buf_len;
		uintptr_t const curr = (uintptr_t) ptr;

                if (!(start <= curr && end > curr)) {
			assert (false && "This pointer doesn't belong to this arena!");
			return NULL;
		}

                if (old_size == new_size) {
			/* Trying to resize the old chunk of mem with the same size */
			return ptr;
		}

                if (curr >= start + (uintptr_t) a->offset){
			/* This is to say that I'm passing a pointer that was already freed, so I don't do anything. */
			return NULL;
		}

                void *new_ptr = arena_stack_alloc_align(a, new_size, align);
		memmove(new_ptr, ptr, min_size);
		return new_ptr;
          }
}

void *arena_resize(stack_arena *a, void* ptr, size_t old_size, size_t new_size) {
	return arena_stack_resize_align(a, ptr, old_size, new_size, DEFAULT_ALIGNMENT);
}

void arena_stack_free_all(stack_arena *a) {
	a->offset = 0;
}

int main (void)
{
	unsigned int const bytes = 1024;
	unsigned char *back_buf = (unsigned char*) malloc(bytes);
	stack_arena arena;

        arena_stack_init(&arena, back_buf, bytes);

	char *name0 = (char *) arena_stack_alloc(&arena, sizeof("Lain"));
	memcpy(name0, "Lain", sizeof("Lain"));

        char *name1 = (char *) arena_stack_alloc(&arena, sizeof("Julia"));
	memcpy(name1, "Julia", sizeof("Julia"));

        char *name2 = (char *) arena_stack_alloc(&arena, sizeof("Eileen"));
	memcpy(name2, "Eileen", sizeof("Eileen"));

        char *name3 = (char *) arena_stack_alloc(&arena, sizeof("Lisa"));
	memcpy(name3, "Lisa", sizeof("Lisa"));

	printf("%s\n", name0);
	printf("%s\n", name1);
	printf("%s\n", name2);
	printf("%s\n", name3);

	arena_stack_free(&arena, (void **) name3);
	/* MANUALLY do this here? */
	/* name3 = NULL; */
	/* printf("%s\n", name3); */

	int *numbers = arena_stack_alloc(&arena, sizeof(int) * 8);
	for (int i = 0; i < 8; ++i)
		numbers[i] = i + 1;

	for (int i = 0; i < 8; ++i)
		printf("%d\n", numbers[i]);

        free(back_buf);
	
	return 0;
}
