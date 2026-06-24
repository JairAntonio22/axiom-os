SECTIONS {
  . = 0x80000000;
  .text : {
    KEEP(*(.text._start));
    *(.text*);
  }
  .rodata : {
    *(.rodata*)
  }
  .data : {
    *(.data*)
  }
  .bss : {
    bss_start = .;
    *(.bss*)
    bss_end = .;
  }
  . = ALIGN(16);
  memory_begin = .;
  . += 4K;
  memory_end = .;
}
