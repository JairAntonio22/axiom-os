# Axiom Learning Notes

This file collects study resources and learning focus for Axiom.

The project is intentionally educational. Agents should recommend learning paths and small exercises rather than jumping straight to implementation.

## Current Learning Focus

Near-term topics:

1. RISC-V CSRs related to traps.
2. RISC-V trap handling in machine mode.
3. Calling convention and trap frame design.
4. Minimal freestanding runtime responsibilities.

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

Before implementing more trap logic, the user should understand:

- What `mtvec` points to.
- What `mcause` encodes.
- Why the top bit of `mcause` distinguishes interrupt vs exception on RV64.
- What `mepc` stores.
- When it is correct to advance `mepc`.
- Why calling C from a trap can corrupt registers if context is not saved.

Suggested trap exercise:

- Trigger an `ecall`.
- Read/report `mcause`, `mepc`, and `mtval`.
- Advance `mepc` only for the expected `ecall` case.
- Halt on unknown exceptions.

## Mentoring Reminder

When the user is stuck, provide hints in levels:

1. Conceptual direction.
2. Concrete checklist.
3. Pseudocode.
4. Minimal snippet.
5. Full implementation only if explicitly requested.
