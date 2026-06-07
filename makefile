.SILENT:

.PHONY: all run clean

all: bin/kernel.elf

QEMU_FLAGS=-display none -serial mon:stdio -M virt -bios bin/kernel.elf

run: bin/kernel.elf
	qemu-system-riscv64 $(QEMU_FLAGS)

clean:
	rm -fr bin
	rm -fr build

# Toolchain

AS=riscv64-elf-as
CC=riscv64-elf-gcc
LD=riscv64-elf-ld

# Assembly files compilation

AS_SRCS=$(shell find src -name '*.s')
AS_OBJS=$(AS_SRCS:src/%.s=build/%.o)

AS_FLAGS=-c -march=rv64i -mabi=lp64

build/%.o: src/%.s
	mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) -o $@ $<

# C files compilation

CC_FLAGS=-c -march=rv64i -mabi=lp64 -mcmodel=medany -Iinclude -std=c99 \
	 -Wall -Wextra -Werror -fno-stack-protector -fno-pic -ffreestanding \
	 -MMD -MP

C_SRCS=$(shell find src -name '*.c')
C_OBJS=$(C_SRCS:src/%.c=build/%.o)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -o $@ $<

# Kernel linking

OBJS=$(AS_OBJS) $(C_OBJS)
DEPS=$(OBJS:.o=.d)

LD_FLAGS=-T scripts/kernel.ld --no-dynamic-linker -m elf64lriscv -static \
	 -nostdlib -s

bin/kernel.elf: $(OBJS)
	mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) -o $@ $(OBJS)

-include $(DEPS)
