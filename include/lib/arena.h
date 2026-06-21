#ifndef ARENA_H
#define ARENA_H

#include <lib/base.h>

typedef struct arena {
	u8 *buf;
	usize len;
	usize cap;
} arena;

void *arena_alloc_align(arena *a, usize num_bytes, usize align);

void *arena_alloc(arena *a, usize num_bytes);

void *arena_zalloc(arena *a, usize num_bytes);

void arena_reset(arena *a);

void arena_zreset(arena *a);

#endif
