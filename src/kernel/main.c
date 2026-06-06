#include <kernel/printf.h>

void kmain() {
	printf("Hello, World!\n");
	printf("\n");

	printf("Hello %x\n", 0);
	printf("Hello %x\n", 5);
	printf("Hello %x\n", 10);
	printf("Hello %x\n", 20);
	printf("\n");

	printf("Hello %c\n", 'a');
	printf("\n");

	printf("Hello %s\n", "string");
}
