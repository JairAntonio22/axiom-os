#include "lib/base.h"
#include <kernel/printf.h>
#include <kernel/kalloc.h>
#include <kernel/trap.h>

#include <drivers/uart.h>

int boot_count = 0;

void kmain()
{
	printf("align_up(0, 16)  = %d\n", align_up(0, 16));
	printf("align_up(1, 16)  = %d\n", align_up(1, 16));
	printf("align_up(15, 16) = %d\n", align_up(15, 16));
	printf("align_up(16, 16) = %d\n", align_up(16, 16));
	printf("align_up(17, 16) = %d\n", align_up(17, 16));
	printf("align_up(31, 16) = %d\n", align_up(31, 16));
	printf("align_up(32, 16) = %d\n", align_up(32, 16));
	printf("align_up(33, 16) = %d\n", align_up(33, 16));

	printf("align_down(0, 16)  = %d\n", align_down(0, 16));
	printf("align_down(1, 16)  = %d\n", align_down(1, 16));
	printf("align_down(15, 16) = %d\n", align_down(15, 16));
	printf("align_down(16, 16) = %d\n", align_down(16, 16));
	printf("align_down(17, 16) = %d\n", align_down(17, 16));
	printf("align_down(31, 16) = %d\n", align_down(31, 16));
	printf("align_down(32, 16) = %d\n", align_down(32, 16));
	printf("align_down(33, 16) = %d\n", align_down(33, 16));

	/*
	trap_init();

	printf("boot count: %d\n", boot_count++);

	__asm__ volatile("ecall");

	printf("after: %d\n", trap_hit);
	*/
}
