#include <lib/arena.h>

void memset(void *mem, u8 val, usize len)
{
	u8 *buf = (u8 *)mem;

	for (usize i = 0; i < len; i++) {
		buf[i] = val;
	}
}

void memzero(void *mem, usize len)
{
	memset(mem, 0, len);
}

void memcpy(void *dst, void *src, usize len)
{
	u8 *buf_dst = (u8 *)dst;
	u8 *buf_src = (u8 *)src;

	for (usize i = 0; i < len; i++) {
		buf_dst[i] = buf_src[i];
	}
}

usize strlen(char *s)
{
	usize len = 0;

	while (*s++) {
		len++;
	}

	return len;
}
