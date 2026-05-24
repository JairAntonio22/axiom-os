#include "unity.c"

void kernel_main() {
	uart_write_cstr("Hello, Wolrd!\n");
}
