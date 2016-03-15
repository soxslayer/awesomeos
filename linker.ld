MEMORY
{
  ext_ram : ORIGIN = 0x80200000, LENGTH = 256M
  l3_ram : ORIGIN = 0x4030000, LENGTH = 64K
}

SECTIONS
{
  __kernel_size__ = 10M;
  __irq_stack_size__ = 1M;
  .init : { *(.init) } > ext_ram
  .vector_table : {
    *(.vector_table)
  } > ext_ram
  .text : { *(.text) } > ext_ram
  .data : {
    __data_start__ = .;
    *(.data)
  } > ext_ram
  __data_end__ = .;
  .rodata : {
    *(.rodata)
  } > ext_ram
  .bss : {
    __bss_start__ = .;
    *(.bss)
  } > ext_ram
  __bss_end__ = .;
  __heap_start__ = .;
  . = 0x80200000 + __kernel_size__;
  __stack_end__ = .;
  __irq_stack_start__ = .;
  . += __irq_stack_size__;
  __irq_stack_end__ = .;
  __fiq_stack_start__ = .;
  . += __irq_stack_size__;
  __fiq_stack_end__ = .;
}
