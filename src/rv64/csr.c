#include <rv64/csr.h>

#define INTERRUPT_BIT (1ULL << 63)

void write_mtvec(uptr val)
{
	__asm__ volatile("csrw mtvec, %0" : : "r"(val));
}

bool mcause_is_interrupt(u64 val)
{
	return (val & INTERRUPT_BIT) != 0;
}

u64 mcause_code(u64 val)
{
	return val & ~INTERRUPT_BIT;
}
