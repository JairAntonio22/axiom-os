#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t usize;
typedef ptrdiff_t isize;
typedef uintptr_t uptr;
typedef intptr_t iptr;

#define KB(num) ((num) * 1024ULL)
#define MB(num) (KB(num) * 1024ULL)
#define GB(num) (MB(num) * 1024ULL)

static inline void assert(bool pred)
{
	if (!(pred)) {
		*(volatile i32 *)0 = 0;
	}
}

static inline u64 align_up(u64 num, u64 aln)
{
	assert(aln && (aln & (aln - 1)) == 0);
	return num + (aln - (num & (aln - 1)));
}

static inline u64 align_down(u64 num, u64 aln)
{
	assert(aln && (aln & (aln - 1)) == 0);
	return num - (num & (aln - 1));
}

#endif
