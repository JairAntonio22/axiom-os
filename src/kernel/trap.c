#include <kernel/trap.h>
#include <kernel/printf.h>

#include <rv64/csr.h>

extern void trap_vector(void);

void trap_init(void)
{
	write_mtvec((uptr)trap_vector);
}

static void trap_dump(trap_frame *tf)
{
	printf("raw mcause: %x\n", tf->mcause);
	printf("interrupt?  %b\n", mcause_is_interrupt(tf->mcause));
	printf("cause code: %d\n", mcause_code(tf->mcause));
	printf("mepc:       %x\n", tf->mepc);
	printf("mtval:      %x\n", tf->mtval);
}

static void trap_panic(trap_frame *tf)
{
	trap_dump(tf);
	for (;;) {
	}
}

static void trap_interrupt(trap_frame *tf)
{
	interrupt_code code = mcause_code(tf->mcause);

	switch (code) {
	default: {
		trap_panic(tf);
	} break;
	}
}

static void trap_exception(trap_frame *tf)
{
	exception_code code = mcause_code(tf->mcause);

	switch (code) {
	case ENVIRONMENT_CALL_FROM_M_MODE: {
		tf->mepc += 4;
	} break;

	default: {
		trap_panic(tf);
	} break;
	}
}

void trap_handler(trap_frame *tf)
{
	if (mcause_is_interrupt(tf->mcause)) {
		trap_interrupt(tf);
	} else {
		trap_exception(tf);
	}
}
