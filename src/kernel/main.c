#include <kernel/printf.h>
#include <kernel/kalloc.h>
#include <kernel/trap.h>

#include <drivers/uart.h>

void kmain()
{
	printf("Hello, World!\n");
}
