#ifndef ARENA_H
#define ARENA_H

#include <lib/base.h>

struct arena {
	u8 *buf;
	usize len;
	usize cap;
};

void *arena_alloc_align(struct arena *a, usize num_bytes, usize align);

void *arena_alloc(struct arena *a, usize num_bytes);

void *arena_zalloc(struct arena *a, usize num_bytes);

void arena_free(struct arena *a);

void arena_zfree(struct arena *a);

#endif
