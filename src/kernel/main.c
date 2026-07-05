#include <drivers/uart.h>

#include <kernel/kalloc.h>
#include <kernel/trap.h>
#include <kernel/timer.h>
#include <kernel/sched.h>
#include <kernel/printf.h>

#include <rv64/context.h>

void task1(void)
{
	while (1) {
		printf("Hello from task 1\n");
		sched_yield();
	}
}

void task2(void)
{
	while (1) {
		printf("Hello from task 2\n");
		sched_yield();
	}
}

void kmain(void)
{
	uart_init();
	kalloc_init();
	trap_init();
	timer_init();

	usize stack_size = KB(16);
	task tasks[2];

	uptr sp1 = (uptr)kmalloc_aln(stack_size, 0x10) + stack_size;

	tasks[0] = (task){ .stack_size = stack_size,
			   .ctx.ra = (uptr)task1,
			   .ctx.sp = sp1 };

	uptr sp2 = (uptr)kmalloc_aln(stack_size, 0x10) + stack_size;

	tasks[1] = (task){ .stack_size = stack_size,
			   .ctx.ra = (uptr)task2,
			   .ctx.sp = sp2 };

	sched_add(tasks[0]);
	sched_add(tasks[1]);

	sched_start();
}
