#ifndef KERNEL_H
#define KERNEL_H

#include <lib.h>

extern uint8_t *memory_begin;
extern uint8_t *memory_end;

void kalloc_init();

void *kmalloc(size_t num_bytes);

void *kzalloc(size_t num_bytes);

void printf(char *fmt, ...);

void panic(char *msg);

#endif
