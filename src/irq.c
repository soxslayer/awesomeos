#include "irq.h"

#include "bits.h"
#include "mem.h"
#include "reg.h"

//#include "sem.h"

static void (*_handlers[INTC_NUM_INT])(int);
//static struct sem_t _handler_sem;

void irq_init()
{
  int i;

  for (i = 0; i < INTC_NUM_INT; ++i)
    _handlers[i] = 0;

  //sem_init(&_handler_sem, 1);
}

int irq_register_handler(int irq, void (*handler)(int))
{
  //sem_take(&_handler_sem);

  if (irq < 0 || irq >= INTC_NUM_INT)
    return -1;

  _handlers[irq] = handler;

  //sem_give(&_handler_sem);

  return 0;
}

void irq_enable(uint32_t state)
{
  uint32_t cpsr;

  asm("mrs %[out], cpsr" : [out] "=r" (cpsr));

  cpsr &= ~(_b(IRQ_BIT) | _b(FIQ_BIT));
  cpsr |= state;

  asm("msr cpsr_c, %[in]" : : [in] "r" (cpsr));

  barrier();
}

uint32_t irq_disable()
{
  uint32_t cpsr;
  uint32_t state = 0;

  asm("mrs %[out], cpsr" : [out] "=r" (cpsr));

  state = cpsr & (_b(IRQ_BIT) | _b(FIQ_BIT));
  cpsr |= _b(IRQ_BIT) | _b(FIQ_BIT);

  asm("msr cpsr_c, %[in]" : : [in] "r" (cpsr));

  barrier();

  return state;
}

/* called from IRQ/FIQ vector handler */
void irq_handler(int irq)
{
  void (*h)(int) = _handlers[irq];

  if (h) h(irq);
}

void fiq_handler(int fiq)
{
}
