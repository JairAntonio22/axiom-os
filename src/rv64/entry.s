.section .text._start
.global _start

_start:
	la sp, stack_top	# setting stack pointer
	mv s0, sp		# setting frame pointer

	# initialize bss
	la t0, bss_start
	la t1, bss_end
bss_init_loop:
	bgeu t0, t1, bss_init_end
	sb zero, 0(t0)
	addi t0, t0, 1
	j bss_init_loop
bss_init_end:

	call kmain		# jumping to kernel main

loop:
	j loop

.section .data
.space 1024 * 8
.align 16

stack_top:
