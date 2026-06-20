#ifndef ARENA_H
#define ARENA_H

#include <lib/base.h>

struct arena {
	uint8_t *buf;
	size_t len;
	size_t cap;
};

void *arena_alloc_align(struct arena *a, size_t num_bytes, size_t align);

void *arena_alloc(struct arena *a, size_t num_bytes);

void *arena_zalloc(struct arena *a, size_t num_bytes);

void arena_free(struct arena *a);

void arena_zfree(struct arena *a);

#endif
