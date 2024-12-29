#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <assert.h>

#define DEFAULT_ALIGNMENT (2 * sizeof(void*))

typedef struct arena arena;

struct arena {
	u_char *buf;
	size_t buf_len;
	size_t prev_offset;
	size_t curr_offset;
};

static bool ispowof2(uintptr_t x) {
	return (x & (x - 1)) == 0;
}

static uintptr_t align_forward(uintptr_t ptr, size_t align) {
	assert(ispowof2(align) && "this ain't a power of 2");
	uintptr_t p = ptr, a = align;
	uintptr_t mod = p & (a - 1);
	if (mod != 0) {
		p += a - mod;
	}
	return p;
}

static void *arena_alloc_align(arena *a, size_t size, size_t align) {
	/* align curr_offset forward */
	uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
	uintptr_t offset = align_forward(curr_ptr, align);
	offset -= (uintptr_t)a->buf;

	if (offset + size > a->buf_len) {
		fprintf(stderr, "No mem available in arena!\n");
		return NULL;
	}
	
	void *ptr = &a->buf[offset];
	a->prev_offset = offset;
	a->curr_offset = offset + size;
	memset(ptr, 0, size);
	return ptr;
}

void arena_init(arena *a, u_char *buf, size_t buf_len) {
	a->buf = buf;
	a->buf_len = buf_len;
	a->curr_offset = a->prev_offset = 0;
}

void *arena_alloc(arena *a, size_t size) {
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

void arena_free(arena* a) {
	a->curr_offset = a->prev_offset = 0;
}

static void *arena_resize_align(arena *a, void *old_m, size_t old_size, size_t new_size, size_t align) {
	assert(ispowof2(align));
	u_char *old_mem = (u_char *)old_m;

	if (old_m == NULL || old_size == 0) {
		return arena_alloc(a, new_size);
	} else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len) {
		/* old memory is part of this arena */
		if (a->buf + a->prev_offset == old_mem) {
			/* is this old memory my last allocation? then resize */
			a->curr_offset = a->prev_offset + new_size;
			if (new_size > old_size) {
				if (a->curr_offset + (new_size - old_size) > a->buf_len) {
					fprintf(stderr, "Out of arena space!");
					return NULL;
				}
				memset(&a->buf[a->curr_offset], 0, new_size - old_size); /* careful not to zero out existing memory */
			}
			return old_mem;
		} else {
			/* old memory is not my previous allocation, so I need to allocate a new
			   chunk of memory at the current position of the arena, then copy the old
			   data into this new location. You don't want to get in here too often bc
			   the old memory location is now effectively wasted. */
			void* new_mem = arena_alloc_align(a, new_size, align);
			size_t copy_size = old_size < new_size ? old_size : new_size;
			memmove(new_mem, old_mem, copy_size);
			return new_mem;
		}
	} else {
		assert(false && "This memory addr doesn't belong to this arena!");
		return NULL;
	}
}

void *arena_resize(arena *a, void *old_mem, size_t old_size, size_t new_size) {
	return arena_resize_align(a, old_mem, old_size, new_size, DEFAULT_ALIGNMENT);
}

int main(void) {
	arena a;
	a.buf = NULL;
	a.buf_len = 0;
	a.curr_offset = 0;
	a.prev_offset = 0;
	a.buf_len = 42;
	
	a.buf = (u_char *) malloc(a.buf_len);
        if (a.buf == NULL) {
		fprintf(stderr, "Couldn't allocate memory\n");
		return 1;
	}

        char* name0 = (char *) arena_alloc(&a, 6);
	assert(name0);
	char* name1 = (char *) arena_alloc(&a, 5);
	assert(name1);
	char* name2 = (char *) arena_alloc(&a, 4);
	assert(name2);
	/* This part fails because it runs out of memory, cool! */
        /* char* name3 = arena_alloc(&a, 4); */
	/* assert(name3); */

        memcpy(name0, "Julia", sizeof("Julia"));
        memcpy(name1, "Lain", sizeof("Lain"));
        memcpy(name2, "Max", sizeof("Max"));
        /* strncpy(name3, "Nix", strlen("Nix")); */

        printf("name is: %s\n", name0);
        printf("name is: %s\n", name1);
        printf("name is: %s\n", name2);
        /* printf("name is: %s\n", name3); */

	/* I don't know how this is very useful (yet) */
	char *name3 = (char *) arena_resize(&a, name2, 4, 7);
	assert(name3);

	memcpy(name3, "Eileen", sizeof("Eileen"));
	printf("after resizing name is %s\n", name3);

	/* Let's try creating a whole different example */
	arena_free(&a);

	/* This is kind of stupid, but it's alright */
	a.buf_len = 1024;
	a.buf = (u_char *) realloc(a.buf, a.buf_len);
	assert(a.buf != NULL);
	arena_init(&a, a.buf, a.buf_len);

	int *nums = (int *) arena_alloc(&a, 5 * sizeof(int));
	for (int i = 0; i < 5; ++i)
		nums[i] = i + 1;

	/* Now I realloc */
	int *new_nums = (int *) arena_resize(&a, nums, 5 * sizeof(int), 10 * sizeof(int));
	for (int i = 0; i < 5; ++i)
		new_nums[i + 5] = i + 5 + 1;

	for (int i = 0; i < 10; ++i)
		printf("%d\n", nums[i]);
	
	free(a.buf);
	return 0;
}
