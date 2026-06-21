#include <kernel/trap.h>
#include <kernel/printf.h>

#include <rv64/csr.h>
#include <stdbool.h>

extern void trap_vector(void);

void trap_init(void)
{
	write_mtvec((uptr)trap_vector);
}

void trap_handler(void)
{
	u64 mcause = read_mcause();
	uptr mepc = read_mepc();

	if (mcause_is_interrupt(mcause)) {
		interrupt_code code = mcause_code(mcause);

		switch (code) {
		default: {
			printf("unhandled interrupt: %d", code);

			while (true) {
			}
		} break;
		}
	} else {
		exception_code code = mcause_code(mcause);

		switch (mcause_code(mcause)) {
		case ENVIRONMENT_CALL_FROM_M_MODE: {
			write_mepc(mepc + 4);
		} break;

		default: {
			printf("unhandled exception: %d", code);

			while (true) {
			}
		} break;
		}
	}
}
