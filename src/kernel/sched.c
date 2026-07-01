#include "rv64/context.h"
#include <lib/status.h>
#include <kernel/sched.h>

#define MAX_TASKS 0x10

static task tasks[MAX_TASKS];
static u64 curr = MAX_TASKS - 1;

extern void context_switch(context *prev, context *next);

static task *sched_select(void)
{
	assert(MAX_TASKS && (MAX_TASKS & (MAX_TASKS - 1)) == 0);

	u64 limit = 0;
	u64 next = curr;

	do {
		next = (next + 1) & (MAX_TASKS - 1);

	} while (tasks[next].state != RUNNABLE && limit++ < MAX_TASKS);

	if (tasks[next].state != RUNNABLE) {
		return NULL;
	}

	return &tasks[next];
}

void sched_start(void)
{
	task *next = sched_select();

	if (!next) {
		return;
	}

	next->state = RUNNING;
	curr = next->id;
	context_switch(NULL, &next->ctx);
}

status_code sched_add(task t)
{
	u64 tid = 0;

	while (tid < MAX_TASKS) {
		if (tasks[tid].state == UNUSED) {
			break;
		}

		tid++;
	}

	if (tid == MAX_TASKS) {
		return EFULL;
	}

	t.id = tid;
	t.state = RUNNABLE;

	tasks[tid] = t;

	return OK;
}

void sched_switch(void)
{
	task *prev = &tasks[curr];
	task *next = sched_select();

	if (!next || prev == next) {
		return;
	}

	prev->state = RUNNABLE;
	next->state = RUNNING;
	curr = next->id;

	context_switch(&prev->ctx, &next->ctx);
}

void sched_yield(void)
{
	sched_switch();
}
