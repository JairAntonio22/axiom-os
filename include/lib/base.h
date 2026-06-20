#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KB(num) ((num) * 1024ULL)
#define MB(num) (KB(num) * 1024ULL)
#define GB(num) (MB(num) * 1024ULL)

static inline void assert(bool pred)
{
	if (!(pred)) {
		*(volatile int32_t *)0 = 0;
	}
}

static inline uint64_t align_up(uint64_t num, uint64_t aln)
{
	assert(aln && (aln & (aln - 1)) == 0);
	return num + (aln - (num & (aln - 1)));
}

static inline uint64_t align_down(uint64_t num, uint64_t aln)
{
	assert(aln && (aln & (aln - 1)) == 0);
	return num - (num & (aln - 1));
}

#endif
