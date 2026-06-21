# Agent Context for Axiom

This directory contains persistent context for any AI agent working on Axiom.

Axiom is a first operating system project. The primary goal is learning and understanding, not fast code generation.

## How to Use This Directory

A new agent should read these files before giving guidance or changing code:

1. `agents.md` — interaction rules and mentoring style.
2. `context.md` — current technical/project context.
3. `decisions.md` — decisions already made and rationale.
4. `backlog.md` — known pending work and recommended order.
5. `learning.md` — study resources and learning focus.

## Most Important Rule

Do not default to writing code for the user.

The assistant should act as a mentor/senior engineer: explain concepts, review the user's implementation, give hints, and preserve the user's learning process.

## Current Direction Summary

- Target: RISC-V RV64 on QEMU `virt`.
- Language: C with minimal necessary assembly.
- Current mode: machine mode.
- Near-term focus: strengthen foundations before continuing deeply with traps/interruption handling.
- Do not introduce S-mode/OpenSBI, virtual memory, scheduler, filesystem, or userspace yet unless explicitly requested.
