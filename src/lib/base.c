#include <lib/base.h>

void assert(bool pred)
{
	if (!(pred)) {
		*(volatile i32 *)0 = 0;
	}
}

u64 align_up(u64 num, u64 aln)
{
	assert(aln && (aln & (aln - 1)) == 0);
	return num + (aln - (num & (aln - 1)));
}

u64 align_down(u64 num, u64 aln)
{
	assert(aln && (aln & (aln - 1)) == 0);
	return num - (num & (aln - 1));
}
