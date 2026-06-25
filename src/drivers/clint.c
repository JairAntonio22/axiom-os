#include <drivers/clint.h>

#include <lib/base.h>

static volatile u64 *mmreg_mtimecmp = (volatile u64 *)(0x02004000);

static volatile u64 *mmreg_mtime = (volatile u64 *)(0x0200bff8);

u64 mtime_read(void)
{
	return *mmreg_mtime;
}

void mtimecmp_write(u64 val)
{
	*mmreg_mtimecmp = val;
}
