#include <kernel/printf.h>
#include <kernel/kalloc.h>
#include <kernel/trap.h>

#include <drivers/uart.h>

void kmain()
{
	trap_init();

	printf("before ecall\n");

	__asm__ volatile(".word 0xffffffff");

	printf("after ecall\n");

	while (true) {
	}
}
