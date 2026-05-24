.PHONY: all run clean

all: bin/kernel

QEMU_FLAGS=-display none -serial mon:stdio

run: bin/kernel
	qemu-system-riscv64 $(QEMU_FLAGS) -machine virt -bios bin/kernel

clean:
	rm -fr bin/*
	rm -fr .build/*

AS_FLAGS=-c -march=rv64i -mabi=lp64

.build/entry.o: src/entry.s
	riscv64-elf-as $(AS_FLAGS) -o .build/entry.o src/entry.s

CC_FLAGS=-c -O0 -march=rv64i -mabi=lp64 -mcmodel=medany

.build/main.o: src/main.c
	riscv64-elf-gcc $(CC_FLAGS) -o .build/main.o src/main.c

LD_FLAGS=--no-dynamic-linker -m elf64lriscv -static -nostdlib -s

bin/kernel: .build/entry.o .build/main.o scripts/kernel.ld
	riscv64-elf-ld $(LD_FLAGS) -T scripts/kernel.ld -o bin/kernel .build/*.o
