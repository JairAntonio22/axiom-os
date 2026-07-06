#include <lib/status.h>

#include <kernel/printf.h>
#include <kernel/sched.h>
#include <kernel/kalloc.h>

#define MAX_TASKS 0x10

static task tasks[MAX_TASKS];
static isize curr = -1;

extern void context_switch(context *prev, context *next);

static task *sched_select(void)
{
	assert(MAX_TASKS && (MAX_TASKS & (MAX_TASKS - 1)) == 0);

	usize limit = 0;
	isize next = curr;

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

static status_code sched_add(task t)
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

static void sched_switch(void);

static void sched_trampoline(void)
{
	if (tasks[curr].state == RUNNING) {
		tasks[curr].entry();
		sched_exit();
	}

	panic("no more tasks to do\n");
}

status_code sched_spawn(task_entry entry, usize stack_size)
{
	uptr stack_base = (uptr)kmalloc_aln(stack_size, 0x10);

	if (!stack_base) {
		return ENOMEM;
	}

	task task = { .entry = entry,
		      .stack_size = stack_size,
		      .ctx.ra = (uptr)sched_trampoline,
		      .ctx.sp = stack_base + stack_size };

	return sched_add(task);
}

static void sched_switch(void)
{
	task *prev = &tasks[curr];
	task *next = sched_select();

	if (!next || prev == next) {
		return;
	}

	next->state = RUNNING;
	curr = next->id;

	context_switch(&prev->ctx, &next->ctx);
}

void sched_yield(void)
{
	tasks[curr].state = RUNNABLE;
	sched_switch();
}

void sched_exit(void)
{
	tasks[curr].state = DONE;
	sched_switch();
}
