# Axiom Decisions

This file records decisions made during mentoring so future agents do not reopen settled topics without reason.

## 1. Assistant Role

Decision:

- The assistant acts as mentor/senior/manager, not as default code author.

Rationale:

- The user is learning OS development through implementation.
- Copy-paste solutions reduce learning value.

Implication:

- Avoid writing implementation code unless explicitly requested.
- Prefer explanations, tasks, review, and hints.

## 2. Learning Workflow

Decision:

- For each new topic, use a research-first workflow before implementation.

Workflow:

1. Suggest focused readings/resources.
2. Give investigation questions.
3. Let the user research and answer.
4. Discuss the answers and design tradeoffs.
5. Implement only after the design is understood.
6. Review and validate the implementation.

Rationale:

- Axiom is primarily an OS learning project.
- Research questions help the user build the mental model before writing code.
- This avoids copy-paste implementation and keeps complexity intentional.

Implication:

- For UART readiness, scheduler design, virtual memory, and other major topics, agents should lead with resources and questions unless the user explicitly asks for code. Stack placement is settled for the current phase and should only be reopened for per-hart/task-stack or runtime model changes.

## 3. Machine Mode for Now

Decision:

- Continue running Axiom in RISC-V machine mode for now.

Rationale:

- This is the user's first OS.
- Machine mode has fewer moving parts initially.
- It allows direct learning of boot, CSRs, traps, UART, and memory.

Future:

- Supervisor mode/OpenSBI may be considered later after machine-mode fundamentals are understood.
- Avoid introducing OpenSBI/S-mode prematurely.

## 4. Assembly Usage

Decision:

- Use assembly only where appropriate/necessary.

Appropriate assembly areas:

- Boot entry.
- Stack setup.
- Trap vectors.
- Context switching in the future.
- CSR operations if inline wrappers are insufficient.

Rationale:

- The user can read assembly but is still learning to write it.
- Most kernel logic should remain in C for clarity.

## 5. No Major Directory Restructure Yet

Decision:

- Keep the current project structure for now.

Rationale:

- The current `include/`, `src/`, `drivers/`, `kernel/`, `lib/`, `rv64/` structure is adequate for this phase.
- Large restructuring would distract from foundational bugs and learning.

Future:

- `src/rv64` may eventually become something like `src/arch/riscv64`, but not yet.

## 6. Refine Foundations Before Advancing

Decision:

- Refine core utilities before moving deeper into traps and interrupts.

Rationale:

- Traps/debugging depend on trustworthy `printf`, alignment helpers, and allocator behavior.
- Bugs in basic utilities can mislead debugging.

Current recommended order:

1. Trap reporting.
2. Correct `mepc` handling.
3. Trap frame design.
4. Interrupts.

Memory-management cleanup is now deferred until memory work resumes.

## 7. Type Aliases

Decision:

- Adopt Rust-style integer aliases in `include/lib/base.h` and use them across the current codebase.

Current aliases:

- `u8`, `u16`, `u32`, `u64`
- `i8`, `i16`, `i32`, `i64`
- `usize`, `isize`
- `uptr`, `iptr`

Guidance:

- Keep plain C `int` where C semantics matter, especially varargs/default promotions.
- Prefer `uptr`/`iptr` for pointer-to-integer conversions.
- During review, correct alias misuse based on semantic intent, not just bit width.

Semantic rules:

- Use `usize` for sizes, lengths, capacities, counts, byte counts, and array/buffer indices.
- Use `isize` for signed relative offsets and pointer differences that may be negative.
- Use `uptr` for addresses represented as integers, pointer-to-integer conversions, address alignment, MMIO base addresses represented numerically, and CSR address values.
- Use `iptr` rarely, only for signed pointer-sized/address-like values.
- Use `u64`/`i64` when the value is exactly a 64-bit integer or architectural register value, not merely because the platform is RV64.
- Use fixed-size aliases such as `u8`, `u16`, `u32`, `u64` when the exact bit width is part of the data format, hardware register, or architecture contract.

Important distinction:

- `usize` means a size/count.
- `uptr` means an address as an integer.
- `u64` means exactly 64 bits.

Future conceptual types may be more valuable than raw aliases:

- physical address type
- virtual address type
- page/frame types

## 8. Type-Oriented C Style

Decision:

- Axiom treats structs, enums, unions, and other domain constructs as ordinary named types at use sites.
- Use lightweight typedef names across the project, including for internal structs.

Rationale:

- Humans reason about domain types and pointers to types, not C syntactic categories like `struct`, `enum`, or `union` at every use site.
- Modern high-level languages generally do not expose these distinctions in everyday type usage.
- Repeating `struct`, `enum`, or `union` adds verbosity without enough value for this codebase.
- Axiom uses C as the implementation language, but the project style should emphasize domain meaning over C syntax ceremony.

Style:

- Use typedefs for structs, enums, and unions that appear in project code.
- Use snake_case type names such as `arena` and future `trap_frame`.
- Do not hide pointers in typedefs; keep pointer usage explicit as `arena *a`.
- Prefer forms like `typedef struct name { ... } name;` so tags remain available if needed.
- At use sites, prefer `arena *a`, `trap_frame *tf`, or `trap_cause cause` instead of `struct arena *a` or `enum trap_cause cause`.
- Let implementation details remain in declarations, not every use site.

## 9. Header Implementation Policy

Decision:

- Do not define `static inline` functions in header files.
- Headers should expose declarations, constants, macros when necessary, and type definitions.
- Function implementations should live in `.c` files so each function has one project-level implementation.

Rationale:

- This keeps headers closer to normal C interfaces.
- It avoids each compilation unit owning a private copy of helper implementations.
- It makes low-level helpers easier to inspect, link, and reason about while learning.

Current application:

- `include/lib/base.h` declares `assert`, `align_up`, and `align_down`.
- `src/lib/base.c` implements those functions.
- `include/rv64/csr.h` declares CSR helper functions.
- `src/rv64/csr.c` implements those functions.

## 10. Build/Run Separation

Decision:

- Keep `Makefile` focused on compilation, linking, dependency generation, and cleanup.
- Put QEMU execution and development helper commands in shell scripts under `scripts/`.

Rationale:

- Build and execution are separate responsibilities.
- The `Makefile` should answer: how is the kernel built?
- Runtime scripts should answer: how the kernel is launched or validated. Debug scripts may be reintroduced later when the debug workflow is clear.

Current scripts:

- `scripts/run.sh`: runs `make` and then executes the kernel in QEMU.
- `scripts/validate.sh`: runs `make clean`, `make`, and verifies `bin/kernel.elf` exists and is non-empty.

Both scripts intentionally use `set -eu` so failures stop immediately and unset variables are treated as errors. `validate.sh` should remain silent on success but print a clear error on failure.

No root-level wrappers are kept for these scripts; commands should call the scripts under `scripts/` directly.

Zed tasks:

- `run`: executes `scripts/run.sh`.
- `build`: executes `make`.
- `validate`: executes `scripts/validate.sh` and uses `hide: "on_success"` so failures remain visible.
- `clean`: executes `make clean`.

Debugger note:

- Zed debugger integration was attempted but deferred because the attach schema expected a local `pid`, while the desired workflow was GDB remote attach to QEMU via `target remote localhost:1234`.
- There is currently no `.zed/debug.json`, no `debug` Zed task, and no debug helper script.
- Reintroduce debug tooling later only when the desired workflow is clear.

Layout note:

- Keeping `scripts/kernel.ld` beside shell helper scripts is acceptable for now because it is still project tooling/build metadata rather than kernel source.
- Do not move `kernel.ld` yet only for aesthetic reasons.
- Reconsider moving the linker script when Axiom supports multiple architectures, platforms, or linker layouts.
- At that point, prefer architecture/target-specific ownership, for example `arch/riscv64/kernel.ld`, `src/rv64/kernel.ld`, or a dedicated `linker/` layout if there are multiple scripts but not yet multiple architectures.

## 11. Build Discipline and Failure UX

Decision:

- The build should be clean, strict, and quiet.
- Axiom follows the principle: silent success, loud failure.

Rationale:

- Warnings often indicate real bugs or unclear intent in low-level code.
- A quiet build makes new warnings/errors stand out immediately.
- The project already uses a silent Makefile style; preserve that signal-to-noise ratio.

Implication:

- Prefer explicit expressions when they avoid warnings or ambiguity.
- Do not normalize warning-filled builds.
- If warning flags are tightened later, code should be adjusted rather than ignoring diagnostics.
- Scripts should produce little or no output on success, but clear actionable output on failure.
- Kernel/user-facing OS behavior should follow the same UX direction where practical: normal expected paths should be quiet; unexpected or dangerous states should report useful diagnostics loudly.

## 12. Backlog Maintenance

Decision:

- `.agent/backlog.md` should reflect current known pending work.

When a user says they addressed items:

- Verify against code.
- Remove only if actually satisfied.
- Keep small caveats out of the backlog unless they are important enough to track.

Example:

- `printf` edge cases like `INT_MIN` may exist, but were not retained in the backlog because the original debug-tool issues were addressed and the backlog should not become noise.


## 13. Initial Stack Placement

Decision:

- Use a dedicated linker-owned `.stack` output section for the initial boot stack.
- Expose `stack_bottom` and `stack_top` from `scripts/kernel.ld`.
- Keep `stack_top` 16-byte aligned before entering C.
- Place `.stack` after `.bss` and before allocator `memory_begin`.
- Keep the current boot stack single-hart and machine-mode only.
- Keep `entry.s` minimal: initialize `sp` from `stack_top`, set `s0`, clear `.bss`, and call `kmain`.
- Use linker `PHDRS` to keep executable text, read-only data, and writable data/stack in separate `LOAD` segments, avoiding accidental `RWE` permissions.

Rationale:

- The stack is runtime scratch space, not initialized `.data`.
- A dedicated `.stack` section makes ownership and bounds explicit without introducing per-hart/task-stack complexity too early.
- Placing `memory_begin` after `.stack` prevents the early allocator from overlapping stack storage.
- Explicit program headers make the ELF layout deterministic instead of relying on accidental padding or orphan-section placement.

Future:

- Revisit when adding multiple harts, scheduler/task stacks, guard pages, virtual memory, or an S-mode/OpenSBI runtime model.
