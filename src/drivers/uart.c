#include <lib/lib.h>
#include <drivers/uart.h>

volatile uint8_t *mmio_uart = (volatile uint8_t*) (0x10000000);

void uart_init(void) {
}

void uart_write_char(char c) {
	*mmio_uart = c;
}

void uart_write_str(char *s) {
	while (*s != '\0') {
		uart_write_char(*s);
		s++;
	}
}
