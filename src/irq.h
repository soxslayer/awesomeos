#pragma once

#include "bits.h"
#include "types.h"

#define IRQ_BIT 7
#define FIQ_BIT 6

#define IRQ_ENABLE 0

void irq_init();
void irq_enable(uint32_t state);
uint32_t irq_disable();

int irq_register_handler(int irq, void (*handler)(int));
