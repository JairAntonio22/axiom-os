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

typedef void (*task_entry)(void);

typedef struct {
	u64 id;
	task_state state;
	task_entry entry;
	usize stack_size;
	context ctx;
} task;

void sched_start(void);

status_code sched_spawn(task_entry te, usize stack_size);

void sched_yield(void);

void sched_exit(void);

#endif
