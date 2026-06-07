#include <drivers/uart.h>

#include <lib/base.h>

volatile uint8_t *mmio_uart = (volatile uint8_t *)(0x10000000);

void uart_init(void)
{
}

void uart_putc(char c)
{
	*mmio_uart = c;
}
