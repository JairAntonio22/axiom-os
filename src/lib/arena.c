#include <lib/arena.h>

#include <lib/memory.h>

void *arena_alloc_align(struct arena *a, size_t num_bytes, size_t align)
{
	size_t total = align_up(num_bytes, align);

	if (a->len + total >= a->cap) {
		return NULL;
	}

	void *ptr = a->buf + a->len;
	a->len += total;
	return ptr;
}

#define DEFAULT_ALIGNMENT 0x10

void *arena_alloc(struct arena *a, size_t num_bytes)
{
	return arena_alloc_align(a, num_bytes, DEFAULT_ALIGNMENT);
}

void *arena_zalloc(struct arena *a, size_t num_bytes)
{
	void *ptr = arena_alloc(a, num_bytes);
	memzero(ptr, num_bytes);
	return ptr;
}

void arena_free(struct arena *a)
{
	a->len = 0;
}

void arena_zfree(struct arena *a)
{
	memzero(a->buf, a->len);
	a->len = 0;
}
