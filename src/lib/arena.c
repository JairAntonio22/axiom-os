#include "lib.h"

void *arena_alloc(struct arena *a, size_t num_bytes) {
	size_t total = align_up(num_bytes, 16);

	if (a->len + total >= a->cap) {
		return NULL;
	}

	void *ptr = a->buf + a->len;
	a->len += total;
	return ptr;
}

void *arena_zalloc(struct arena *a, size_t num_bytes) {
	void *ptr = arena_alloc(a, num_bytes);
	memzero(ptr, num_bytes);
	return ptr;
}

void arena_free(struct arena *a) {
	a->len = 0;
}

void arena_zfree(struct arena *a) {
	memzero(a->buf, a->cap);
	a->len = 0;
}
