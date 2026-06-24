# Axiom Project Context

This file summarizes the project state and conversation context for future agents.

## Project Purpose

Axiom is an experimental 64-bit operating system for RISC-V, written in C.

It is heavily inspired by Unix, Plan 9, and small systems that can be understood by one person.

The user's goal is to learn operating system development. This is their first OS project, so correctness and understanding matter more than speed or completeness.

## Current Technical Target

- ISA/architecture: RISC-V RV64.
- Current build flags: `rv64imac_zicsr_zifencei`, ABI `lp64`.
- Execution environment: QEMU `virt`.
- Current privilege mode: machine mode.
- Language: C.
- Assembly usage: boot entry and trap vector only unless needed.
- Runtime: freestanding, no libc, no external runtime support.
- Build preference: clean, strict, and quiet; warnings should be treated as issues to understand and fix.
- Build/run separation: `Makefile` owns compilation/linking/cleaning; scripts under `scripts/` own validation and QEMU execution.
- Integer style: Rust-style aliases from `include/lib/base.h` (`u8`, `u64`, `usize`, `uptr`, etc.) are used in project code.
- Struct style: use lightweight typedef names, e.g. `arena *a` instead of `struct arena *a`.

## Current Repository Structure

Relevant structure observed:

```text
include/
  drivers/
  kernel/
  lib/

src/
  drivers/
  kernel/
  lib/
  rv64/

scripts/
  kernel.ld
  run.sh
  validate.sh

.zed/
  tasks.json

.agent/
  readme.md
  agents.md
  backlog.md
  context.md
  decisions.md
  learning.md
```

## Current Implemented Pieces

Known implemented pieces:

- RISC-V boot entry.
- Stack setup.
- Kernel entry point.
- UART output.
- Minimal `printf`.
- Basic memory helpers.
- Basic arena-style allocator.
- Linker script.
- Trap vector with minimal trap frame.
- Trap reporting for `mcause`, `mepc`, and `mtval`.
- Controlled M-mode `ecall` return path.

## Recently Addressed Items

These were previously in the backlog and have been mostly addressed:

- `printf` decimal off-by-one/index bug.
- `%d` varargs mismatch for `int`.
- `%c` varargs mismatch for promoted `int`.
- `panic` interpreting the panic message as a format string.
- `arena_zalloc` zeroing a null pointer after failed allocation.
- Alignment helpers revised with non-zero power-of-two assertion.
- `arena_alloc_align` now aligns the returned pointer using padding.
- Arena capacity check now allows filling the arena exactly.
- Rename from `makefile` to `Makefile`.
- Header guards for `uart.h` and `base.h`.
- README architecture updated from `rv64i` to `rv64imac_zicsr_zifencei`.
- Rust-style integer aliases adopted across current code.
- Struct typedef style adopted for existing structs.
- QEMU execution moved out of `Makefile` into `scripts/run.sh`.
- Validation helper added as `scripts/validate.sh`.
- Runtime scripts are canonical under `scripts/`; no root-level wrappers are kept.
- Zed tasks include `run`, `build`, `validate`, and `clean`.
- Debugger setup was attempted and deferred; there is currently no `.zed/debug.json`, no debug task, and no debug helper script.
- Trap vector now saves/restores general-purpose register context before calling C.
- `trap_frame` captures `mcause`, `mepc`, and `mtval` and is passed to `trap_handler`.
- `trap_handler` distinguishes interrupts from exceptions, advances `mepc` only for handled M-mode `ecall`, and halts on unknown traps.

## Current Known Risk Areas

Important unresolved technical risks:

- Runtime trap validation still relies on manual QEMU triggers rather than a formal test harness.
- Interrupt handling is not implemented yet beyond classification and safe halt behavior.
- `.bss` clearing is not explicitly guaranteed by startup code.
- `kmain` return behavior should be explicit.
- `compile_commands.json` may be stale and still use `rv64i`.
- README build/run/validate instructions should match `Makefile`, `scripts/run.sh`, `scripts/validate.sh`, and Zed tasks. Debug instructions are deferred until the workflow is reintroduced.

Deferred memory-management issues:

- Allocation-size/capacity arithmetic should eventually guard against integer overflow.
- Decide whether early kernel allocation should return `NULL` or panic on failure.

## Important Mentoring Context

The user explicitly requested that the assistant should not simply write code.

The assistant should guide, review, explain, and give tasks. The user should implement the code themselves unless they explicitly request a concrete implementation.

When giving help, prefer:

1. Conceptual explanation.
2. Why the issue matters.
3. What to inspect or change.
4. Criteria for success.
5. Hints only if needed.

## Suggested Next Step

Next focus:

1. Define runtime initialization policy.
2. Explicitly clear `.bss` before entering C.
3. Define what happens if `kmain` returns.
4. Then continue toward timer interrupts.
