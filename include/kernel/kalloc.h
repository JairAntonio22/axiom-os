#ifndef KALLOC_H
#define KALLOC_H

#include <lib/base.h>

void kalloc_init(void);

void *kmalloc(usize num_bytes);

void *kmalloc_aln(usize num_bytes, u64 align);

#endif
