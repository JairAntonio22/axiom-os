#include "kernel.h"

void kmain() {
	kalloc_init();
	printf("Hello, World!");
}
