#!/bin/sh
set -eu

make clean
make

if [ ! -s bin/kernel.elf ]; then
    echo "validate: bin/kernel.elf is missing or empty" >&2
    exit 1
fi
