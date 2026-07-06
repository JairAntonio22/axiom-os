#include <drivers/uart.h>

#include <kernel/kalloc.h>
#include <kernel/trap.h>
#include <kernel/timer.h>
#include <kernel/sched.h>
#include <kernel/printf.h>

#include <rv64/context.h>

void task1(void)
{
	printf("Hello from task 1\n");
	sched_yield();
	printf("Goodbye from task 1\n");
}

void task2(void)
{
	for (int i = 0; i < 10; i++) {
		printf("Hello from task 2\n");
		sched_yield();
	}

	printf("Goodbye from task 2\n");
}

void kmain(void)
{
	uart_init();
	kalloc_init();
	trap_init();
	timer_init();

	sched_spawn(task1, KB(16));
	sched_spawn(task2, KB(16));

	sched_start();
}
