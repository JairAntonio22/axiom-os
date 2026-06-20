#include <kernel/printf.h>
#include <kernel/kalloc.h>
#include <kernel/trap.h>

#include <drivers/uart.h>

void kmain()
{
	trap_init();
	__asm__ volatile("ecall");
}
