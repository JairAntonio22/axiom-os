#ifndef KALLOC_H
#define KALLOC_H

#include <lib/base.h>

void kalloc_init(void);

void *kmalloc(usize num_bytes);

void *kzalloc(usize num_bytes);

#endif
