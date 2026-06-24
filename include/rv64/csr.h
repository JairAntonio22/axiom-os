#ifndef CSR_H
#define CSR_H

#include <lib/base.h>

// CSR read & write

void write_mtvec(uptr val);

// mcause utils

bool mcause_is_interrupt(u64 val);

u64 mcause_code(u64 val);

#endif
