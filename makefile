.SILENT:

.PHONY: all run clean

all: bin/kernel

QEMU_FLAGS=-display none -serial mon:stdio

run: bin/kernel
	qemu-system-riscv64 $(QEMU_FLAGS) -machine virt -bios bin/kernel

clean:
	rm -fr bin/*
	rm -fr .build/*

TC_PREFIX=riscv64-elf-

AS=$(TC_PREFIX)as
CC=$(TC_PREFIX)gcc
LD=$(TC_PREFIX)ld

AS_FLAGS=-c -march=rv64i -mabi=lp64

.build/entry.o: src/rv64/entry.s
	$(AS) $(AS_FLAGS) -o .build/entry.o src/rv64/entry.s

CC_FLAGS=-c -O0 -march=rv64i -mabi=lp64 -mcmodel=medany -Isrc -std=c99 \
	 -Wall -Wextra -Werror -fno-stack-protector -fno-pic -ffreestanding \

.build/kernel.o: src/**/*.c src/**/*.h
	$(CC) $(CC_FLAGS) -o .build/kernel.o src/build.c

LD_FLAGS=--no-dynamic-linker -m elf64lriscv -static -nostdlib -s

bin/kernel: .build/entry.o .build/kernel.o scripts/kernel.ld
	$(LD) $(LD_FLAGS) -T scripts/kernel.ld -o bin/kernel .build/*.o
