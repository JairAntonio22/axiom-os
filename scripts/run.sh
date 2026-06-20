#!/bin/sh
set -eu

make
exec qemu-system-riscv64 \
    -display none \
    -serial mon:stdio \
    -M virt \
    -bios bin/kernel.elf
