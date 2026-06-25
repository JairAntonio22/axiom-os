#include <kernel/timer.h>

#include <rv64/csr.h>
#include <drivers/clint.h>

#define TIMER_INTERVAL 100000ULL

static volatile u64 count;

static void timer_sched(void)
{
	u64 mtime = mtime_read();
	mtimecmp_write(mtime + TIMER_INTERVAL);
}

void timer_init(void)
{
	timer_sched();
	mie_enable_mtie();
	mstatus_enable_mie();
}

void timer_intr(void)
{
	count++;
	timer_sched();
}

u64 timer_count(void)
{
	return count;
}
