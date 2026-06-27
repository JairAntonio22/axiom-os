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

## Use of AI

Axiom is developed with assistance from AI tools.

This is intentional and disclosed openly. The goal of the project is learning operating system development, not pretending that every explanation, review, or edit happened in isolation.

AI is used primarily as a mentor and review partner:

- explaining RISC-V and operating-system concepts;
- helping break large topics into small, verifiable steps;
- reviewing code for bugs, unclear behavior, and design risks;
- maintaining project notes and backlog context;
- making focused edits when explicitly requested.

AI is not treated as an oracle or as a substitute for understanding the system. Design decisions, code that enters the kernel, and the responsibility for correctness remain with the project author.

The `.agent/` directory contains persistent context for AI-assisted development. If it appears in the repository, it should be understood as part of the project's transparent development process, not as hidden authorship or an attempt to misrepresent the work.

---

## Current Status

Axiom is currently in very early development.

Implemented so far:

- RISC-V RV64 bare-metal boot on QEMU `virt`
- Basic kernel entry point
- Custom linker script with explicit `LOAD` segment permissions
- Dedicated linker-owned boot stack section
- Explicit `.bss` clearing before entering C
- Polling NS16550A-compatible UART input/output
- Trap handling with a minimal trap frame
- Machine timer interrupts through QEMU `virt` CLINT
- Basic arena-style allocator

Current focus:

- Scheduler design research, without implementation yet

Planned later:

- Physical memory management improvements
- Virtual memory
- Cooperative multitasking / scheduling
- Filesystem
- Userspace programs

---

## Architecture

Current target:

- ISA: RISC-V RV64
- Architecture flags: `rv64imac_zicsr_zifencei`
- ABI: `lp64`
- Language: C with minimal assembly for boot and traps
- Execution environment: QEMU `virt`
- Current privilege mode: machine mode
- Boot flow: QEMU loads the kernel ELF as firmware → `_start` → `kmain`

The project intentionally avoids unnecessary dependencies and large frameworks.

---

## Building and Running

Requirements:

- RISC-V GNU toolchain
- QEMU for RISC-V

Build the kernel:

```bash
make
```

Run under QEMU:

```bash
scripts/run.sh
```

Validate a clean build:

```bash
scripts/validate.sh
```

Clean build artifacts:

```bash
make clean
```

The repository does not track editor-local task files. If you use Zed or another editor, keep those files local unless the project intentionally adopts shared editor configuration.

---

## Toolchain

Axiom currently uses a minimal bare-metal RISC-V toolchain setup:

- `riscv64-elf-as`
- `riscv64-elf-gcc`
- `riscv64-elf-ld`

Target configuration:

- Architecture: `rv64imac_zicsr_zifencei`
- ABI: `lp64`

The kernel is linked statically without libc or external runtime support.

Execution currently targets the QEMU `virt` machine. The canonical run command lives in `scripts/run.sh` and currently executes QEMU with `bin/kernel.elf` as firmware:

```bash
qemu-system-riscv64 \
    -display none \
    -serial mon:stdio \
    -M virt \
    -bios bin/kernel.elf
```

Debugger integration is intentionally deferred for now.

---

## Repository Hygiene

Tracked project files should be source, documentation, or reproducible project configuration.

Generated/local artifacts are intentionally ignored, including:

- `bin/`
- `build/`
- `.cache/`
- `.zed/`
- `compile_commands.json`
- `*.dtb`

`compile_commands.json` can be useful for LSP/editor integration, but it is generated locally and may contain machine-specific paths. Regenerate it when needed, for example with:

```bash
bear -- make
```

The human-readable QEMU `virt` device tree snapshot may be kept as documentation under `docs/`, but the binary `.dtb` form is treated as generated output.

---

## Non-Goals

Axiom is not trying to become:

- a Linux replacement
- a compatibility layer for existing ecosystems
- a feature-complete modern desktop OS
- a production-ready kernel

Complexity is easy to accumulate.

The difficult part is preserving clarity while the system grows.
