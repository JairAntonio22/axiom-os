#ifndef MEM_H
#define MEM_H

#include <lib/base.h>

void memset(void *mem, uint8_t val, size_t len);

void memzero(void *mem, size_t len);

void memcpy(void *dst, void *src, size_t len);

size_t strlen(char *s) ;

#endif
