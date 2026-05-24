.section .text._start
.global _start

_start:
	add s0, sp, zero
	la sp, __stack_top
	jal zero, kernel_main

loop:
	j loop

.section .data
.space 1024 * 8
.align 16

__stack_top:
