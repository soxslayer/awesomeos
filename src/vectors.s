.global vectors_init

.data
#VECTOR_TABLE = 0x4030ce24
VECTOR_TABLE = 0xffff0000

INTC_BASE = 0x48200000

INTC_SIR_IRQ = 0x40
INTC_SIR_FIQ = 0x44
INTC_CONTROL = 0x48

.section .vector_table, "awx"
  .align 5
__vector_table:
@ reset
b __empty_handler
@ undefined instruction
b __empty_handler
@ software interrupt
b __empty_handler
@ prefetch abort
b __empty_handler
@ data abort
b __empty_handler_repeat
@ hypervisor trap
b __empty_handler
@ IRQ
b __handle_irq
@ FIQ
b __empty_handler_repeat

__handle_irq:
  push {r0-r12, lr}

  ldr r4, =INTC_BASE

  ldr r0, [r4, #INTC_SIR_IRQ]
  and r0, r0, #0x7f

  ldr r1, =irq_handler
  blx r1

  ldr r0, [r4, #INTC_CONTROL]
  orr r0, #0x1
  str r0, [r4, #INTC_CONTROL]

  pop {r0-r12, lr}

  subs pc, lr, #4

@__fiq_handler:
@  push {r0-r12}
@
@  ldr r0, =INTC_SIR_FIQ
@  and r0, r0, #0x7f
@
@  ldr r1, =irq_handler
@  blx r1
@
@  ldr r0, =INTC_CONTROL
@  orr r0, #0x2
@
@  pop {r0 - r12}
@
@  subs pc, lr, #4

__empty_handler:
  subs pc, r14, #0

__empty_handler_repeat:
  subs pc, r14, #4

vectors_init:
  ldr r0, =__vector_table
  mcr p15, 0, r0, c12, c0, 0

  dsb

  bx lr
