.section .text

.align 2
.global trap_vector

trap_vector:
	addi sp, sp, -0x110	# allocate trap frame

	# Save original context
	# x0 is not saved because it is hardwired to zero
	sd ra,  0x00(sp)
	sd t0,  0x20(sp)	# save t0 before using it as scratch
	addi t0, sp, 0x110	# t0 = original sp
	sd t0,  0x08(sp)
	sd gp,  0x10(sp)
	sd tp,  0x18(sp)
	sd t1,  0x28(sp)
	sd t2,  0x30(sp)
	sd s0,  0x38(sp)
	sd s1,  0x40(sp)
	sd a0,  0x48(sp)
	sd a1,  0x50(sp)
	sd a2,  0x58(sp)
	sd a3,  0x60(sp)
	sd a4,  0x68(sp)
	sd a5,  0x70(sp)
	sd a6,  0x78(sp)
	sd a7,  0x80(sp)
	sd s2,  0x88(sp)
	sd s3,  0x90(sp)
	sd s4,  0x98(sp)
	sd s5,  0xa0(sp)
	sd s6,  0xa8(sp)
	sd s7,  0xb0(sp)
	sd s8,  0xb8(sp)
	sd s9,  0xc0(sp)
	sd s10, 0xc8(sp)
	sd s11, 0xd0(sp)
	sd t3,  0xd8(sp)
	sd t4,  0xe0(sp)
	sd t5,  0xe8(sp)
	sd t6,  0xf0(sp)

	csrr t0, mcause
	sd t0,  0xf8(sp)
	csrr t0, mepc
	sd t0,  0x100(sp)
	csrr t0, mtval
	sd t0,  0x108(sp)

	# Trap handler function call
	mv a0, sp
	call trap_handler

	# Apply CSR changes made by the trap handler
	ld t0,  0x100(sp)
	csrw mepc, t0

	# Restore original context.
	ld ra,  0x00(sp)
	ld gp,  0x10(sp)
	ld tp,  0x18(sp)
	ld t0,  0x20(sp)
	ld t1,  0x28(sp)
	ld t2,  0x30(sp)
	ld s0,  0x38(sp)
	ld s1,  0x40(sp)
	ld a0,  0x48(sp)
	ld a1,  0x50(sp)
	ld a2,  0x58(sp)
	ld a3,  0x60(sp)
	ld a4,  0x68(sp)
	ld a5,  0x70(sp)
	ld a6,  0x78(sp)
	ld a7,  0x80(sp)
	ld s2,  0x88(sp)
	ld s3,  0x90(sp)
	ld s4,  0x98(sp)
	ld s5,  0xa0(sp)
	ld s6,  0xa8(sp)
	ld s7,  0xb0(sp)
	ld s8,  0xb8(sp)
	ld s9,  0xc0(sp)
	ld s10, 0xc8(sp)
	ld s11, 0xd0(sp)
	ld t3,  0xd8(sp)
	ld t4,  0xe0(sp)
	ld t5,  0xe8(sp)
	ld t6,  0xf0(sp)

	# Free trap frame, this restores the interrupted sp
	addi sp, sp, 0x110

	mret
