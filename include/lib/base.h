#ifndef LIB_H
#define LIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KB(num) ((num) * 1024ULL)
#define MB(num) (KB(num) * 1024ULL)
#define GB(num) (MB(num) * 1024ULL)

#define assert(pred) \
	if (!(pred)) \
		*(volatile int32_t *)0 = 0;

#define align_up(num, aln) (1 + (num | (num ^ (aln - 1))))
#define align_down(num, aln) (1 + ((num & (aln - 1) | (aln - 1))

#endif
