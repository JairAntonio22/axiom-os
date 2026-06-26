PHDRS {
  text PT_LOAD FLAGS(5);
  rodata PT_LOAD FLAGS(4);
  data PT_LOAD FLAGS(6);
}

SECTIONS {
  . = 0x80000000;

  .text : ALIGN(4K) {
    KEEP(*(.text._start));
    *(.text*)
  } :text

  .rodata : ALIGN(4K) {
    *(.rodata*)
  } :rodata

  .data : ALIGN(4K) {
    *(.sdata)
    *(.sdata.*)
    *(.data)
    *(.data.*)
  } :data

  .bss : ALIGN(4K) {
    bss_start = .;
    *(.sbss)
    *(.sbss.*)
    *(.bss)
    *(.bss.*)
    *(COMMON)
    bss_end = .;
  } :data

  .stack (NOLOAD) : ALIGN(16) {
    stack_bottom = .;
    . += 16K;
    . = ALIGN(16);
    stack_top = .;
  } :data

  . = ALIGN(4K);
  memory_begin = .;
  . += 256K;
  memory_end = .;
}
