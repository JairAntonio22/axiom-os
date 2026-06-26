#include <kernel/kalloc.h>
#include <kernel/trap.h>
#include <kernel/timer.h>
#include <kernel/printf.h>

void kmain(void)
{
	kalloc_init();
	trap_init();
	timer_init();
	printf("running");
}
