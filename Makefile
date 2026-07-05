.SILENT:

.PHONY: all clean

all: bin/kernel.elf

clean:
	rm -fr bin
	rm -fr build
	rm -f a.out

# Toolchain

ifeq ($(filter clean,$(MAKECMDGOALS)),)
ifndef CROSS_COMPILE
$(error CROSS_COMPILE is not set. Example: make CROSS_COMPILE=riscv64-unknown-elf-)
endif
endif

AS=$(CROSS_COMPILE)as
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld

# Architecture and ABI

ARCH=-march=rv64imac_zicsr_zifencei -mabi=lp64

# Assembly files compilation

AS_SRCS=$(shell find src -name '*.s')
AS_OBJS=$(AS_SRCS:src/%.s=build/%.o)

build/%.o: src/%.s
	mkdir -p $(dir $@)
	$(AS) $(ARCH) -g -c -o $@ $<

# C files compilation

CC_FLAGS=-g -c -mcmodel=medany -Iinclude -std=c99 -Wall -Wextra -MMD -MP \
	-fno-stack-protector -fno-pic -ffreestanding

C_SRCS=$(shell find src -name '*.c')
C_OBJS=$(C_SRCS:src/%.c=build/%.o)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(ARCH) $(CC_FLAGS) -o $@ $<

# Kernel linking

OBJS=$(AS_OBJS) $(C_OBJS)
DEPS=$(OBJS:.o=.d)

LD_FLAGS=-T scripts/kernel.ld --no-dynamic-linker -m elf64lriscv -static \
	 -nostdlib -s

bin/kernel.elf: $(OBJS)
	mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) -o $@ $(OBJS)

-include $(DEPS)
