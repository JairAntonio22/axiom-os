#include <kernel/kalloc.h>
#include <kernel/printf.h>

void kmain()
{
	kalloc_init();
}
