#ifndef TRAP_FRAME_H
#define TRAP_FRAME_H

#include <lib/base.h>

typedef struct {
	uptr ra; // 0x00
	uptr sp; // 0x08
	uptr gp; // 0x10
	uptr tp; // 0x18
	u64 t0; // 0x20
	u64 t1; // 0x28
	u64 t2; // 0x30
	u64 s0; // 0x38
	u64 s1; // 0x40
	u64 a0; // 0x48
	u64 a1; // 0x50
	u64 a2; // 0x58
	u64 a3; // 0x60
	u64 a4; // 0x68
	u64 a5; // 0x70
	u64 a6; // 0x78
	u64 a7; // 0x80
	u64 s2; // 0x88
	u64 s3; // 0x90
	u64 s4; // 0x98
	u64 s5; // 0xa0
	u64 s6; // 0xa8
	u64 s7; // 0xb0
	u64 s8; // 0xb8
	u64 s9; // 0xc0
	u64 s10; // 0xc8
	u64 s11; // 0xd0
	u64 t3; // 0xd8
	u64 t4; // 0xe0
	u64 t5; // 0xe8
	u64 t6; // 0xf0
	u64 mcause; // 0xf8
	uptr mepc; // 0x100
	u64 mtval; // 0x108
} trap_frame;

#endif
