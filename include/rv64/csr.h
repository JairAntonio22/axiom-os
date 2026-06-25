#ifndef CSR_H
#define CSR_H

#include <lib/base.h>

// CSR read & write

void mtvec_write(uptr val);

void mstatus_enable_mie(void);

void mie_enable_mtie(void);

// mcause utils

bool mcause_is_interrupt(u64 val);

u64 mcause_code(u64 val);

#endif
