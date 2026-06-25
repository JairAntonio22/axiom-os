#include <kernel/kalloc.h>

#include <lib/arena.h>

extern u8 memory_begin;
extern u8 memory_end;

arena kalloc;

void kalloc_init(void)
{
	kalloc.buf = &memory_begin;
	kalloc.cap = &memory_end - &memory_begin;
}

void *kmalloc(usize num_bytes)
{
	return arena_alloc(&kalloc, num_bytes);
}

void *kzalloc(usize num_bytes)
{
	return arena_zalloc(&kalloc, num_bytes);
}
