#ifndef TIMER_H
#define TIMER_H

#include <lib/base.h>

void timer_init(void);

void timer_intr(void);

u64 timer_count(void);

#endif
