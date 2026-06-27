# Axiom Learning Notes

This file collects study resources and learning focus for Axiom.

The project is intentionally educational. Agents should recommend learning paths and small exercises rather than jumping straight to implementation.

## Current Learning Focus

Near-term topics:

1. Scheduler design research, without implementation yet.
2. Continue reinforcing RISC-V traps, interrupts, calling convention, linker layout, and MMIO basics as needed.

Memory alignment and arena allocator basics were recently refined; deeper allocator hardening is deferred until memory management work resumes.

## Integer Alias Usage

Axiom uses Rust-style aliases from `include/lib/base.h`.

Quick guide:

| Alias | Meaning | Use for |
|---|---|---|
| `usize` | unsigned size/count | lengths, capacities, byte counts, indexes |
| `isize` | signed size/difference | relative offsets, pointer differences that may be negative |
| `uptr` | unsigned pointer-sized integer | addresses represented as integers, pointer casts, address alignment |
| `iptr` | signed pointer-sized integer | rare signed address-like values |
| `u8`/`u16`/`u32`/`u64` | exact unsigned widths | hardware/data values where bit width matters |
| `i8`/`i16`/`i32`/`i64` | exact signed widths | signed values where bit width matters |

Rules of thumb:

- Use `usize` when asking: how many bytes/items?
- Use `uptr` when asking: what is the numeric value of this address?
- Use `u64` when asking: is this exactly a 64-bit value/register?
- Keep plain C `int` where C semantics matter, especially varargs/default promotions.

Agents should correct misuse during reviews because the user expects to make mistakes with these aliases while learning.

## Recommended Study Resources

### RISC-V Privileged Specification

Use as the primary reference for privilege modes and traps.

- https://riscv.org/technical/specifications/

Important topics:

- machine mode
- supervisor mode, later only
- `mtvec`
- `mcause`
- `mepc`
- `mtval`
- `mstatus`
- `mie`
- `mip`
- `mret`

Do not try to read the full spec in one sitting. Use it as a reference.

### RISC-V Assembly Programmer's Manual

Useful for assembly syntax, registers, pseudo-instructions, and conventions.

- https://github.com/riscv-non-isa/riscv-asm-manual

Important topics:

- register names
- pseudo-instructions
- stack usage
- function calls

### RISC-V psABI

Useful for calling convention and register preservation rules.

- https://github.com/riscv-non-isa/riscv-elf-psabi-doc

Important topics:

- caller-saved registers
- callee-saved registers
- argument registers
- return values
- stack alignment

### xv6-riscv

Useful as a reference kernel, especially for conceptual OS organization.

- https://github.com/mit-pdos/xv6-riscv
- https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf

Caution:

- xv6-riscv runs in supervisor mode and uses OpenSBI.
- Axiom currently runs in machine mode.
- Do not copy xv6 mechanisms blindly.

Most relevant chapter soon:

- Traps and system calls.

### Stephen Marz RISC-V OS Blog

Practical RISC-V OS development resource.

- https://osblog.stephenmarz.com/

Useful topics:

- boot
- UART
- memory
- traps

### OSDev Wiki

Useful for general OS concepts, but many pages are x86-oriented.

- https://wiki.osdev.org/

Use for concepts, not architecture-specific RISC-V details.

### Operating Systems: Three Easy Pieces

Useful later for high-level OS concepts.

- https://pages.cs.wisc.edu/~remzi/OSTEP/

Read when approaching:

- processes
- scheduling
- virtual memory
- concurrency
- filesystems

## Exercises for Current Backlog

### Alignment Helpers

Before implementation, manually verify expected behavior for a power-of-two alignment such as 16:

```text
align_up(0, 16)  = 0
align_up(1, 16)  = 16
align_up(15, 16) = 16
align_up(16, 16) = 16
align_up(17, 16) = 32
align_up(31, 16) = 32
align_up(32, 16) = 32
align_up(33, 16) = 48
```

Questions for the user:

- Should `align` be required to be a power of two?
- Should invalid alignment assert/panic?
- Should alignment helpers be macros or functions?

### Arena Allocator

Questions for the user:

- Does this allocator return `NULL` on failure or panic?
- Should returned addresses be aligned, or only allocation sizes?
- What happens if the buffer start is not aligned?
- Is this allocator a temporary bootstrap allocator or the long-term kernel heap?

### Traps

Current status:

- Basic trap reporting, minimal trap frame, controlled M-mode `ecall`, and machine timer interrupt handling are implemented for the current phase.

Keep reinforcing:

- What `mtvec` points to.
- What `mcause` encodes.
- Why the top bit of `mcause` distinguishes interrupt vs exception on RV64.
- What `mepc` stores.
- When it is correct to advance `mepc`.
- Why calling C from a trap can corrupt registers if context is not saved.

### Initial Stack Placement

Current status:

- Completed for the current single-hart M-mode foundation.
- The boot stack is owned by the linker layout through a dedicated `.stack` output section.
- `scripts/kernel.ld` exposes `stack_bottom` and `stack_top`; `stack_top` is 16-byte aligned before entering C.
- `.stack` is placed after `.bss` and before `memory_begin`, keeping allocator memory after the boot stack.
- `entry.s` only initializes `sp` from `stack_top`; it does not reserve stack bytes itself.
- Linker `PHDRS` keep `RX`, `R`, and `RW` load segments separate to avoid accidental `RWE`.
- Revisit this topic when adding per-hart stacks, task stacks, preemption, or privilege-mode changes.

Reference resources:

- RISC-V psABI stack alignment requirements:
  https://github.com/riscv-non-isa/riscv-elf-psabi-doc
- Linker script references:
  - GNU ld linker scripts documentation
  - OSDev Wiki linker script overview: https://wiki.osdev.org/Linker_Scripts
- Existing Axiom files:
  - `src/rv64/entry.s`
  - `scripts/kernel.ld`

Resolved answers:

1. The boot stack is its own special linker-owned region, represented by `.stack`.
2. The current entry path does not clear `.stack`; only `.bss` is explicitly zeroed.
3. A dedicated `.stack` section is clearer than hiding stack storage in `.data` or `.bss`.
4. The linker exposes `stack_bottom` and `stack_top`; a separate `stack_size` symbol is not needed yet.
5. `stack_top` is 16-byte aligned before calling C.
6. The current design is intentionally single-hart for now; per-hart stacks are future work.
7. `memory_begin` starts after `.stack`, so the allocator does not overlap boot stack storage.
8. Success for this phase means a clean build, no accidental `RWE` load segment, and a minimal linker/entry design that is easy to revisit later.

### UART Readiness and Register Layout

Current status:

- Completed for the current polling foundation.
- QEMU `virt` UART is treated as NS16550A-compatible byte-addressed MMIO at `0x10000000`.
- Register access uses `volatile u8 *` so array indexing advances one byte per register offset.
- `RBR`/`THR`, `LCR`, and `LSR` offsets are explicitly named.
- `uart_putc` waits for `LSR.THRE` before writing.
- `uart_getc` waits for `LSR.DR` before reading.
- Interrupt-driven UART, buffering, line discipline, and error handling are deferred.

Reference resources:

- QEMU `virt` DTS UART node: `serial@10000000`, compatible `ns16550a`.
- NS16550A / 16550 UART register references.
- OSDev Wiki serial ports page for conceptual register overview, noting that many examples are x86-port-I/O oriented:
  https://wiki.osdev.org/Serial_Ports
- Existing Axiom files:
  - `src/drivers/uart.c`
  - `include/drivers/uart.h`

Resolved answers:

1. `THR` and `RBR` use offset `0`.
2. `LSR` uses offset `5` for QEMU `virt` byte-addressed NS16550A MMIO.
3. `LSR.THRE` is bit 5; `LSR.DR` is bit 0.
4. `uart_putc` busy-waits until transmit-ready for the current phase.
5. If the UART never becomes ready, the current polling driver intentionally spins; timeout/error recovery is deferred.
6. Axiom defines named constants for register offsets and status bits.
7. UART remains a minimal polling driver for now; interrupt-driven UART is deferred.
8. This is reliable enough for current scheduler/timer debugging output.

### Scheduler Design Research

Do not implement yet. The goal is to build the mental model before writing context-switching code.

Resources:

- OSTEP process and scheduling chapters:
  https://pages.cs.wisc.edu/~remzi/OSTEP/
- xv6-riscv book sections on scheduling/context switching, with caution that xv6 runs in S-mode and uses a different full design:
  https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf
- RISC-V psABI register convention.
- Existing Axiom trap frame and timer interrupt code.

Questions for the user:

1. What is the smallest useful definition of a task/thread in Axiom?
2. What state must be saved to pause one kernel thread and resume another?
3. How is a scheduler context switch different from returning from a trap using `trap_frame`?
4. Should the first scheduler be cooperative, timer-preemptive, or a staged path from cooperative to preemptive?
5. Does each task need its own kernel stack from the beginning?
6. Where would task stacks come from: static arrays, arena allocator, or a future page allocator?
7. What does the timer interrupt need to do before preemption is safe?
8. What invariants must hold while switching stacks?
9. What should happen if a timer interrupt arrives while already inside kernel code?
10. What success criteria prove the first scheduler milestone without adding userspace, VM, or filesystems?

## Mentoring Reminder

When the user is stuck, provide hints in levels:

1. Conceptual direction.
2. Concrete checklist.
3. Pseudocode.
4. Minimal snippet.
5. Full implementation only if explicitly requested.
