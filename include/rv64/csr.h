#ifndef CSR_H
#define CSR_H

#include <lib/base.h>

// CSR read & write

void write_mtvec(uptr val);

u64 read_mcause(void);

uptr read_mepc(void);

void write_mepc(uptr val);

u64 read_mtval(void);

// mcause utils

bool mcause_is_interrupt(u64 val);

u64 mcause_code(u64 val);

#endif
