#include "base.c"

volatile u8 *mmio_uart = (volatile u8*) (0x10000000);

void uart_write_char(char c) {
	*mmio_uart = c;
}

void uart_write_cstr(char *s) {
	while ((*mmio_uart = *s++) != 0);
}
