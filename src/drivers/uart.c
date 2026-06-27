#include <drivers/uart.h>

#include <lib/base.h>

static volatile u8 *mmio_uart = (volatile u8 *)(0x10000000);

#define RBR 0
#define THR 0

#define LCR 3
#define LCR_DLAB (1 << 7)

#define LSR 5
#define LSR_DR (1 << 0)
#define LSR_THRE (1 << 5)

void uart_init(void)
{
	mmio_uart[LCR] &= ~LCR_DLAB;
}

void uart_putc(char c)
{
	while ((mmio_uart[LSR] & LSR_THRE) == 0)
		;

	mmio_uart[THR] = c;
}

char uart_getc(void)
{
	while ((mmio_uart[LSR] & LSR_DR) == 0)
		;

	return mmio_uart[RBR];
}
