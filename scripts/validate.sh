#!/bin/sh
set -eu

make clean
make
test -s bin/kernel.elf
