#ifndef SCHED_H
#define SCHED_H

#include <lib/base.h>
#include <lib/status.h>

#include <rv64/context.h>

typedef enum {
	UNUSED,
	RUNNABLE,
	RUNNING,
	DONE,
} task_state;

typedef struct {
	u64 id;
	task_state state;
	usize stack_size;
	context ctx;
} task;

void sched_start(void);

status_code sched_add(task t);

void sched_yield(void);

#endif
