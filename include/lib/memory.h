#ifndef MEM_H
#define MEM_H

#include <lib/base.h>

void memset(void *mem, u8 val, usize len);

void memzero(void *mem, usize len);

void memcpy(void *dst, void *src, usize len);

usize strlen(char *s);

#endif
