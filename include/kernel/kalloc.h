#ifndef KALLOC_H
#define KALLOC_H

#include <lib/base.h>

void kalloc_init();

void *kmalloc(size_t num_bytes);

void *kzalloc(size_t num_bytes);

#endif
