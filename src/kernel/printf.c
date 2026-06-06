#include <drivers/uart.h>

#include <lib/base.h>
#include <stdarg.h>

void print_number(uint64_t n);

void printf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	while (*fmt) {
		if (*fmt != '%') {
			uart_write_char(*fmt++);
			continue;
		}

		char argtype = fmt[1];

		switch (argtype) {
			case '\0': {
			} break;

			case 'x': {
				uint64_t arg = va_arg(args, uint64_t);
				print_number(arg);
				fmt += 2;
			} break;

			case 'c': {
				uint64_t arg = va_arg(args, uint64_t);
				uart_write_char(arg);
				fmt += 2;
			} break;

			case 's': {
				char *arg = va_arg(args, char*);
				uart_write_str(arg);
				fmt += 2;
			} break;

			default: {
				uint64_t arg = va_arg(args, uint64_t);
				uart_write_char(arg);
				fmt++;
			} break;
				
		}
	}

	va_end(args);
}

void print_number(uint64_t n) {
	uart_write_str("0x");

	uint64_t offset = 60;

	for (int i = 0; i < 16; i++) {
		uint8_t byte = (n & (0xf << offset)) >> offset;

		if (byte < 10) {
			uart_write_char(byte + '0');

		} else {
			uart_write_char((byte - 10) + 'a');
		}

		offset -= 4;
	}

}

void panic(char *msg) {
	printf(msg);
	assert(false);
}
