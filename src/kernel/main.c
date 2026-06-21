#include <kernel/printf.h>
#include <kernel/kalloc.h>
#include <kernel/trap.h>

#include <drivers/uart.h>

void kmain()
{
	trap_init();

	/*
	panic("entering trap vector");

	entering trap vector
	mcause:     0x0000000000000007
	interrupt?  false
	cause code: 0x0000000000000007
	mepc:       0x0000000080000378
	mtval::     0x0000000000000000

	*/

	printf("before ecall\n");

	__asm__ volatile("ecall");

	printf("after ecall\n");

	/*
	mcause:     0x000000000000000b
	interrupt?  false
	cause code: 0x000000000000000b
	mepc:       0x0000000080000832
	mtval:      0x0000000000000000

	*/

	/*
	__asm__ volatile(".word 0x00000000");

	mcause:     0x0000000000000002
	interrupt?  false
	cause code: 0x0000000000000002
	mepc:       0x0000000080000832
	mtval:      0x0000000000000000

	*/
	while (true) {
	}
}
