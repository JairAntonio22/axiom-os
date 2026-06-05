#ifndef LIB_H
#define LIB_H

// BASE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KB(num) ((num) * 1024ULL)
#define MB(num) (KB(num) * 1024ULL)
#define GB(num) (MB(num) * 1024ULL)

#define assert(pred) if (!(pred)) *(volatile int32_t *) 0 = 0;

#define align_up(num, aln) (1 + (num | (num ^ (aln - 1))))
#define align_down(num, aln) (1 + ((num & (aln - 1) | (aln - 1))

// MEMORY

void memset(void *mem, uint8_t val, size_t len);

void memzero(void *mem, size_t len);

void memcpy(void *dst, void *src, size_t len);

size_t strlen(char *s) ;

// ARENA

struct arena {
	uint8_t *buf;
	size_t len;
	size_t cap;
};

void *arena_alloc(struct arena *a, size_t num_bytes);

void *arena_zalloc(struct arena *a, size_t num_bytes);

void arena_free(struct arena *a);

void arena_zfree(struct arena *a);

#endif
