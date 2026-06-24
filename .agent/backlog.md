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

- Timer interrupt research: QEMU `virt` timer source, required CSRs, and reprogramming strategy.

## Todo

- Revisit initial stack placement and ownership.
- UART readiness and register layout.
- README build/run/validate instructions.
- Revisit Zed debugger integration for QEMU remote GDB attach.

## Testing

- `compile_commands.json` sync.
- Runtime trap validation in QEMU:
  - Confirm expected M-mode `ecall` returns after advancing `mepc`.
  - Confirm an unexpected trap prints `mcause`, `mepc`, and `mtval`, then halts.
  - Keep artificial trap triggers out of normal boot once validation is complete.

## Blocked

These are intentionally deferred until prerequisites are understood.

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
- Trap reporting for `mcause`, `mepc`, and `mtval`.
- Trap cause decoding distinguishes interrupts from exceptions.
- `mepc` is advanced only for handled M-mode `ecall`.
- Unknown exceptions and unhandled interrupts halt instead of silently continuing.
- Minimal trap frame added and used by the trap vector.
- Trap vector preserves general-purpose register context before calling C.
- Runtime entry explicitly clears `.bss` before entering C.
- If `kmain` returns, `entry.s` falls back to a known infinite loop.
- Initial allocator memory range is separated from `.bss` in the linker script.

## Board Notes

- Keep stack placement as a follow-up; the current stack works but still lives in `.data`.
- Timer interrupts are the next active research area now that basic runtime initialization is explicit.
- Keep this board in sync with the priority sections below.

---

# Priority 1: Trap Handling Foundations

Status: completed for the current bare-metal foundation.

Files:

- `src/kernel/trap.c`
- `src/rv64/trap_vector.s`
- `include/kernel/trap.h`
- `include/rv64/trap_frame.h`
- `include/rv64/csr.h`

Current state:

- `mtvec` is set to `trap_vector`.
- The trap vector builds a minimal `trap_frame` on the current stack.
- General-purpose registers are saved before calling C and restored before `mret`.
- `mcause`, `mepc`, and `mtval` are captured in the trap frame.
- `trap_handler` distinguishes interrupts from synchronous exceptions.
- M-mode `ecall` is handled by advancing `mepc` by 4.
- Unknown exceptions halt after dumping trap details.
- Unhandled interrupts halt after dumping trap details.
- The trap handler may update `tf->mepc`, and the trap vector writes it back before returning.

Validation notes:

- Build validation should continue to use `scripts/validate.sh`.
- Runtime validation can use QEMU via `scripts/run.sh`.
- Expected trap behavior to preserve:
  - An intentional M-mode `ecall` returns to the next instruction.
  - Unexpected traps print useful diagnostic state.
  - Unknown traps do not silently continue.
  - Interrupts are not treated like exceptions and do not receive unconditional `mepc += 4`.

Remaining future caveats:

- The current trap frame is sufficient for early M-mode kernel work, but may need revision when adding nested traps, preemption, privilege transitions, or per-hart stacks.
- Interrupt handling is not implemented yet beyond classification and safe halt behavior.
- Timer interrupt work should come next only after runtime initialization is made explicit.
- More complete diagnostics may later decode known exception and interrupt names in human-readable form.
- Trap behavior should be revalidated whenever the entry path, stack layout, calling convention assumptions, or CSR handling changes.

Resources:

- RISC-V Privileged Specification:
  https://riscv.org/technical/specifications/
- RISC-V Assembly Programmer's Manual:
  https://github.com/riscv-non-isa/riscv-asm-manual
- RISC-V ELF psABI:
  https://github.com/riscv-non-isa/riscv-elf-psabi-doc

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

Status: completed for the current foundation, with stack placement left as a follow-up.

Current state:

- `entry.s` sets the initial stack before entering C.
- `entry.s` clears `.bss` byte-by-byte using `bss_start` and `bss_end` from the linker script.
- `entry.s` calls `kmain`, so a return from `kmain` reaches the fallback loop.
- `scripts/kernel.ld` exposes `bss_start`, `bss_end`, `memory_begin`, and `memory_end`.
- `memory_begin` and `memory_end` are outside `.bss`.

Remaining follow-up:

- Revisit initial stack placement and ownership. The current stack is defined in `.data`; a future cleanup may move it to a clearer stack-specific area once the desired memory layout is better understood.

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

# Recommended Order Before Continuing Deeply With Interrupts

1. Research timer interrupt foundations:
   - identify the QEMU `virt` timer source,
   - identify the required machine-mode CSRs and bits,
   - decide how the next tick is programmed.
2. Add timer interrupt foundations:
   - enable only the required machine-mode interrupt bits,
   - program the QEMU `virt` timer source deliberately,
   - handle timer interrupts without advancing `mepc`,
   - keep diagnostics available for unexpected interrupt state.
3. Revalidate trap behavior after timer work:
   - expected `ecall` still returns correctly,
   - unknown exceptions still halt,
   - unhandled interrupts do not silently continue.

Do not prioritize yet:

- virtual memory,
- scheduler,
- filesystem,
- userspace,
- OpenSBI/S-mode migration,
- large directory restructuring.

---

# Current Decision

Trap handling foundations are complete enough for the current phase.

The project does not need a major rewrite.

The next technical work should focus on runtime initialization policy: explicit `.bss` clearing and defined `kmain` return behavior. After that, continue toward timer interrupts while keeping VM, scheduler, filesystem, userspace, and OpenSBI/S-mode work deferred.
