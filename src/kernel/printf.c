#include <lib/base.h>
#include <drivers/uart.h>
#include <stdarg.h>

void print_number(uint64_t n);

void printf(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	while (*fmt) {
		if (*fmt != '%') {
			uart_putc(*fmt++);
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
			uart_putc(arg);
			fmt += 2;
		} break;

		case 's': {
			char *arg = va_arg(args, char *);

			while (*arg) {
				uart_putc(*arg++);
			}

			fmt += 2;
		} break;

		default: {
			uart_putc(*fmt++);
		} break;
		}
	}

	va_end(args);
}

void print_number(uint64_t n)
{
	uart_putc('0');
	uart_putc('x');

	uint64_t offset = 60;

	for (int i = 0; i < 16; i++) {
		uint8_t byte = (n & (0xfULL << offset)) >> offset;

		if (byte < 10) {
			uart_putc(byte + '0');

		} else {
			uart_putc((byte - 10) + 'a');
		}

		offset -= 4;
	}
}

void panic(char *msg)
{
	printf(msg);
	assert(false);
}
