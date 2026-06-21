#include <rv64/csr.h>

#define INTERRUPT_BIT (1ULL << 63)

void write_mtvec(uptr val)
{
	__asm__ volatile("csrw mtvec, %0" : : "r"(val));
}

u64 read_mcause(void)
{
	u64 val;
	__asm__ volatile("csrr %0, mcause" : "=r"(val));
	return val;
}

uptr read_mepc(void)
{
	uptr val;
	__asm__ volatile("csrr %0, mepc" : "=r"(val));
	return val;
}

void write_mepc(uptr val)
{
	__asm__ volatile("csrw mepc, %0" : : "r"(val));
}

u64 read_mtval(void)
{
	u64 val;
	__asm__ volatile("csrr %0, mtval" : "=r"(val));
	return val;
}

bool mcause_is_interrupt(u64 val)
{
	return (val & INTERRUPT_BIT) != 0;
}

u64 mcause_code(u64 val)
{
	return val & ~INTERRUPT_BIT;
}
