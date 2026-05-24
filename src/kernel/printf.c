#include <kernel/kernel.h>
#include <drivers/uart.h>
#include <stdarg.h>

void printf(char *fmt, ...) {
	size_t argc = 0;

	for (size_t i = 0; fmt[i]; i++) {
		argc += fmt[i] == '%';
	}

	if (!argc) {
		uart_write_str(fmt);
		uart_write_char('\n');
		return;
	}

	va_list args;
	va_start(args, fmt);

	for (size_t i = 0; i < argc; i++) {
		char *msg = va_arg(args, char*);
		uart_write_str(msg);
	}

	uart_write_char('\n');
	va_end(args);
}

void panic(char *msg) {
	printf(msg);
	assert(false);
}
