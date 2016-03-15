.global init2

.section .init
init1:
  @ save CPSR register
  mrs r0, cpsr

  @ initialize IRQ mode
  msr cpsr_c, #0x12
  ldr sp, =__irq_stack_end__

  @ initialize FIQ mode
  msr cpsr_c, #0x11
  ldr sp, =__fiq_stack_end__

  @ initialize supervisor mode
  msr cpsr_c, r0
  ldr sp, =__stack_end__

  ldr r1, =__bss_start__
  ldr r2, =__bss_end__
  mov r0, #0

bss_clr:
  cmp r1, r2
  strlt r0, [r1], #4
  blt bss_clr

  ldr r4, =init2
  mov lr, sp
  bx r4

infinity:
  b infinity
