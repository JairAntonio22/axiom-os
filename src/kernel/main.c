#include <drivers/uart.h>
#include <kernel/kalloc.h>
#include <kernel/trap.h>
#include <kernel/timer.h>
#include <kernel/printf.h>

void kmain(void)
{
	uart_init();
	kalloc_init();
	trap_init();
	timer_init();
}
