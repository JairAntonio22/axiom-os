#include <kernel/trap.h>

#include <lib/base.h>
#include <kernel/printf.h>
#include <stdint.h>

extern void trap_vector(void);
void trap_handler(void);

void trap_init(void)
{
	uint64_t addr = (uint64_t)trap_vector;
	__asm__ volatile("csrw mtvec, %0" : : "r"(addr));
}

volatile int trap_hit = 0;

void trap_handler(void)
{
	trap_hit = 1234;
}
