# Axiom Backlog

This backlog captures the current state before continuing with traps and interrupts.

## Current State

Axiom currently has:

- RISC-V RV64 bare-metal boot.
- QEMU `virt` execution.
- Basic kernel entry.
- UART output.
- Minimal `printf`.
- Basic arena-style allocator.
- Kernel linker script.
- Early trap vector.
- Experimental `ecall` handling.

Current build architecture in `Makefile`:

- `rv64imac_zicsr_zifencei`
- ABI: `lp64`

The README is partially updated, but the explicit QEMU command should still be checked against `Makefile`.

---

# Work Board

This board is intentionally high-level. Keep it updated when tasks move between states.

## In Progress

- Trap reporting: `mcause`, `mepc`, `mtval`

## Todo

- Stop advancing `mepc` unconditionally.
- Design minimal trap frame.
- Runtime init policy: `.bss` clearing and `kmain` return behavior.
- UART readiness and register layout.
- README build/run/validate instructions.
- Revisit Zed debugger integration for QEMU remote GDB attach.

## Testing

- `compile_commands.json` sync.

## Blocked

These are intentionally deferred until prerequisites are understood.

- Timer interrupts.
- Virtual memory.
- Scheduler.
- Filesystem.
- Userspace.
- S-mode/OpenSBI migration.

## Done

- `printf` basic fixes.
- `align_up` and `align_down`.
- `arena_alloc_align` returned pointer alignment.
- Arena exact-capacity allocation.
- `Makefile` rename.
- Header guards cleanup.
- Rust-style integer aliases adopted in current codebase.
- Struct typedef style adopted for existing structs.
- Runtime helper scripts moved under `scripts/`.
- Added `scripts/validate.sh` as the canonical full validation command.
- Added Zed `validate` task.
- Root-level wrappers intentionally not kept.
- Debug task/scripts and `.zed/debug.json` intentionally not kept for now.

## Board Notes

- Move trap reporting to `Testing` once `mcause`, `mepc`, and `mtval` are printed and explained.
- Move `mepc` handling to `Done` only after unknown traps halt instead of silently continuing.
- Keep this board in sync with the priority sections below.

---

# Priority 1: Trap Handling Foundations

Files:

- `src/kernel/trap.c`
- `src/kernel/trap_vector.s`
- `include/kernel/trap.h`

Current state:

- `mtvec` is set to `trap_vector`.
- `ecall` reaches the trap handler.
- `trap_handler` only sets a global value.
- `trap_vector` always advances `mepc` by 4.
- Registers are not preserved before calling C.

## Issues

### Trap vector does not preserve interrupted context

Calling C from a trap without saving registers can corrupt the interrupted code.

Need to study:

- RISC-V calling convention.
- caller-saved registers.
- callee-saved registers.
- trap frames.
- stack alignment.

Resources:

- https://github.com/riscv-non-isa/riscv-asm-manual
- https://github.com/riscv-non-isa/riscv-elf-psabi-doc

### `mepc` is advanced unconditionally

This is only appropriate for expected synchronous exceptions like `ecall`.

It is not generally correct for:

- illegal instructions,
- memory faults,
- misaligned accesses,
- interrupts,
- unknown exceptions.

Goal:

- Read and report `mcause`, `mepc`, and `mtval`.
- Distinguish interrupt vs exception.
- Advance `mepc` only when appropriate.
- Halt on unknown exceptions.

Success criteria:

- An `ecall` is identified correctly.
- The handler prints useful trap information.
- Unknown traps stop the kernel instead of silently continuing.
- Interrupts do not incorrectly advance `mepc`.

Resource:

- RISC-V Privileged Specification:
  https://riscv.org/technical/specifications/

Important CSRs:

- `mtvec`
- `mcause`
- `mepc`
- `mtval`
- `mstatus`
- `mie`
- `mip`

---

# Priority 2: Runtime Initialization

Files:

- `src/rv64/entry.s`
- `scripts/kernel.ld`

Issues:

- `.bss` clearing is not explicitly handled.
- `kmain` may return, but kernel entry should define what happens if it does.
- Stack setup is minimal.

Goals:

- Understand minimal freestanding runtime responsibilities.
- Decide whether `kmain` must never return.
- Eventually clear `.bss` explicitly before entering C.

Success criteria:

- Global zero-initialized data is reliable by design, not accident.
- If `kmain` returns, the kernel enters a known halt loop.

---

# Priority 3: UART Improvements

Files:

- `src/drivers/uart.c`
- `include/drivers/uart.h`

Current state:

- UART writes directly to MMIO base address.
- This works for basic QEMU output.

Potential improvements:

- Wait for transmitter readiness before writing.
- Clarify UART register layout.

Priority:

- Low for now.
- Useful later if output becomes unreliable.

---

# Priority 4: Project Hygiene

## README

File:

- `README.md`

Outdated details:

- Build/run/validate instructions should match `Makefile`, scripts under `scripts/`, and `.zed/tasks.json`.
- Debug instructions are deferred until debug scripts/tasks are reintroduced.

Priority:

- Low-medium.

## `compile_commands.json`

Issue:

- Uses stale flags such as `-march=rv64i`.
- May not match `Makefile`.

Impact:

- Editor diagnostics may differ from real build behavior.

Priority:

- Medium for development experience.

---

# Priority 5: Future Memory Management

Files:

- `include/lib/base.h`
- `src/lib/arena.c`
- `include/lib/arena.h`
- future memory-management files

Notes:

- Alignment helpers have been revised and now assert non-zero power-of-two alignment.
- `arena_alloc_align` now aligns the returned pointer using padding.
- The arena capacity check now allows filling the arena exactly.

Future issues to revisit when returning to memory management:

- Allocation-size/capacity arithmetic should eventually guard against integer overflow.
- Decide whether early kernel allocation should return `NULL` or panic on failure.
- Decide whether the arena is only a bootstrap allocator or part of the longer-term kernel heap.

Priority:

- Defer until memory management work resumes.

---

# Recommended Order Before Continuing Deeply With Traps

1. Improve trap reporting:
   - `mcause`
   - `mepc`
   - `mtval`
2. Stop advancing `mepc` unconditionally.
3. Study and design a minimal trap frame.
4. Then continue toward interrupts.

Do not prioritize yet:

- virtual memory,
- scheduler,
- filesystem,
- userspace,
- OpenSBI/S-mode migration,
- large directory restructuring.

---

# Current Decision

Refine the foundation before advancing.

The project does not need a major rewrite.

The next technical work should return to trap reporting and `mepc` handling. Tooling is currently sufficient for that phase: use `scripts/validate.sh` for clean validation and `scripts/run.sh` for QEMU execution.
