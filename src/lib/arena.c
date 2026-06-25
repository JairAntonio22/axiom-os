#include <lib/arena.h>
#include <lib/memory.h>

void *arena_alloc_align(arena *a, usize num_bytes, usize align)
{
	void *cursor = a->buf + a->len;
	uptr ptr = align_up((uptr)cursor, align);
	usize pad = ptr - (uptr)cursor;

	if (a->len + pad + num_bytes > a->cap) {
		return NULL;
	}

	a->len += pad + num_bytes;
	return (void *)ptr;
}

#define DEFAULT_ALIGNMENT 0x10

void *arena_alloc(arena *a, usize num_bytes)
{
	return arena_alloc_align(a, num_bytes, DEFAULT_ALIGNMENT);
}

void *arena_zalloc(arena *a, usize num_bytes)
{
	void *ptr = arena_alloc(a, num_bytes);

	if (!ptr) {
		return NULL;
	}

	memzero(ptr, num_bytes);
	return ptr;
}

void arena_reset(arena *a)
{
	a->len = 0;
}

void arena_zreset(arena *a)
{
	memzero(a->buf, a->len);
	a->len = 0;
}
