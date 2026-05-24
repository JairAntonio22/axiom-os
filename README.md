# Axiom

> A small and comprehensible operating system for RISC-V.

Axiom is an experimental 64-bit operating system written in C for RISC-V.

The project is heavily inspired by classic Unix systems, Plan 9, and the idea that software should remain understandable at a human scale. Modern systems accumulated enormous complexity over decades of compatibility, abstraction layers, and tooling. Axiom explores a different direction: coherent design, explicit behavior, and conceptual simplicity.

The goal is not to compete with production operating systems.

The goal is to recover the feeling that a computer can still be understood as a whole.

---

## Philosophy

Axiom is built around a few core ideas:

- Small systems are easier to reason about.
- Simplicity is a design constraint, not a lack of features.
- The entire stack should remain inspectable.
- Software should expose mechanisms clearly instead of hiding them behind layers.
- Good abstractions reduce complexity instead of relocating it.
- Human comprehension matters.

The project takes strong inspiration from systems and people that valued clarity over accumulation, including Unix and Plan 9, and engineers such as Rob Pike, Casey Muratori, and Jonathan Blow.

---

## Current Status

Axiom is currently in very early development.

Implemented so far:

- RISC-V boot process
- Basic kernel entry point
- UART output
- Custom linker script
- Bare-metal execution under QEMU

Planned:

- Physical memory management
- Virtual memory
- Trap and interrupt handling
- Cooperative multitasking
- Filesystem
- Userspace programs
- Simple toolchain and development environment

---

## Architecture

Current target:

- ISA: RISC-V RV64
- Language: C
- Execution environment: QEMU `virt`
- Boot flow: machine mode → kernel

The project intentionally avoids unnecessary dependencies and large frameworks.

---
## Building

Requirements:

- RISC-V GNU toolchain
- QEMU for RISC-V

Build the kernel:

```bash
make
```

Run under QEMU:

```bash
make run
```

Clean build artifacts:

```bash
make clean
```

---

## Toolchain

Axiom currently uses a minimal bare-metal RISC-V toolchain setup:

- `riscv64-elf-as`
- `riscv64-elf-gcc`
- `riscv64-elf-ld`

Target configuration:

- Architecture: `rv64i`
- ABI: `lp64`

The kernel is linked statically without libc or external runtime support.

Execution currently targets the QEMU `virt` machine:

```bash
qemu-system-riscv64 -machine virt -bios bin/kernel
```

---

## Non-Goals

Axiom is not trying to become:

- a Linux replacement
- a compatibility layer for existing ecosystems
- a feature-complete modern desktop OS
- a production-ready kernel

Complexity is easy to accumulate.

The difficult part is preserving clarity while the system grows.
