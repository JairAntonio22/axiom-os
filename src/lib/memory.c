#include <lib/arena.h>

void memset(void *mem, uint8_t val, size_t len)
{
	uint8_t *buf = (uint8_t *)mem;

	for (size_t i = 0; i < len; i++) {
		buf[i] = val;
	}
}

void memzero(void *mem, size_t len)
{
	memset(mem, 0, len);
}

void memcpy(void *dst, void *src, size_t len)
{
	uint8_t *buf_dst = (uint8_t *)dst;
	uint8_t *buf_src = (uint8_t *)src;

	for (size_t i = 0; i < len; i++) {
		buf_dst[i] = buf_src[i];
	}
}

size_t strlen(char *s)
{
	size_t len = 0;

	while (*s++) {
		len++;
	}

	return len;
}
