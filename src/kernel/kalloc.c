#include <kernel/kalloc.h>

#include <lib/arena.h>

extern uint8_t memory_begin;
extern uint8_t memory_end;

struct arena kalloc;

void kalloc_init()
{
	kalloc.buf = &memory_begin;
	kalloc.cap = &memory_end - &memory_begin;
}

void *kmalloc(size_t num_bytes)
{
	return arena_alloc(&kalloc, num_bytes);
}

void *kzalloc(size_t num_bytes)
{
	return arena_zalloc(&kalloc, num_bytes);
}
