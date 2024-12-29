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

void *arena_alloc(arena *a, size_t size) {
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

int main(void) {
	arena a;
	a.buf = NULL;
	a.buf_len = 0;
	a.curr_offset = 0;
	a.prev_offset = 0;
	
	a.buf = malloc(40);
	a.buf_len = 40;
        if (a.buf == NULL) {
		fprintf(stderr, "Couldn't allocate memory\n");
		return 1;
	}

        char* name0 = arena_alloc(&a, 6);
	assert(name0);
	char* name1 = arena_alloc(&a, 5);
	assert(name1);
	char* name2 = arena_alloc(&a, 4);
	assert(name2);
	/* This part fails because it runs out of memory, cool! */
        /* char* name3 = arena_alloc(&a, 4); */
	/* assert(name3); */

        strncpy(name0, "Julia", strlen("Julia"));
        strncpy(name1, "Lain", strlen("Lain"));
        strncpy(name2, "Max", strlen("Max"));
        /* strncpy(name3, "Nix", strlen("Nix")); */

        printf("name is: %s\n", name0);
        printf("name is: %s\n", name1);
        printf("name is: %s\n", name2);
        /* printf("name is: %s\n", name3); */
	
	free(a.buf);
	return 0;
}
