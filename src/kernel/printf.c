#include <kernel/printf.h>

#include <lib/base.h>
#include <drivers/uart.h>
#include <stdarg.h>

static void print_hex(u64 n)
{
	uart_putc('0');
	uart_putc('x');

	u64 offset = 60;

	for (int i = 0; i < 16; i++) {
		u8 nibble = (n & (0xfULL << offset)) >> offset;

		if (nibble < 10) {
			uart_putc(nibble + '0');

		} else {
			uart_putc((nibble - 10) + 'a');
		}

		offset -= 4;
	}
}

static void print_num(int n)
{
	if (!n) {
		uart_putc('0');
		return;
	}

	bool is_negative = n < 0;

	if (is_negative) {
		uart_putc('-');
		n *= -1;
	}

	char digits[15];
	int i = 0;

	while (n) {
		char d = n % 10 + '0';
		digits[i++] = d;
		n /= 10;
	}

	i--;

	while (i >= 0) {
		uart_putc(digits[i--]);
	}
}

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
			u64 arg = va_arg(args, u64);
			print_hex(arg);
			fmt += 2;
		} break;

		case 'd': {
			int arg = va_arg(args, int);
			print_num(arg);
			fmt += 2;
		} break;

		case 'c': {
			int arg = va_arg(args, int);
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

		case 'b': {
			int arg = va_arg(args, int);
			char *txt = arg ? "true" : "false";

			while (*txt) {
				uart_putc(*txt++);
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

void panic(char *msg)
{
	printf("%s\n", msg);
	assert(false);
}
