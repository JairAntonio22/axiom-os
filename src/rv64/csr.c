#include <rv64/csr.h>

#define INTERRUPT_BIT (1ULL << 63)
#define MSTATUS_MIE (1ULL << 3)
#define MIE_MTIE (1ULL << 7)

void mtvec_write(uptr val)
{
	__asm__ volatile("csrw mtvec, %0" : : "r"(val));
}

bool mcause_is_interrupt(u64 val)
{
	return (val & INTERRUPT_BIT) != 0;
}

void mstatus_enable_mie(void)
{
	__asm__ volatile("csrs mstatus, %0" : : "r"(MSTATUS_MIE));
}

void mie_enable_mtie(void)
{
	__asm__ volatile("csrs mie, %0" : : "r"(MIE_MTIE));
}

u64 mcause_code(u64 val)
{
	return val & ~INTERRUPT_BIT;
}
