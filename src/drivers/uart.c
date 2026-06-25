#include <drivers/uart.h>

#include <lib/base.h>

static volatile u8 *mmio_uart = (volatile u8 *)(0x10000000);

void uart_init(void)
{
}

void uart_putc(char c)
{
	*mmio_uart = c;
}
