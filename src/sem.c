#include "sem.h"
#include "interrupts.h"

/* TODO temporary placeholders until sems implemented */

void sem_init(struct sem_t *sem, int count)
{
  sem->count = count;
}

int sem_take(struct sem_t *sem)
{
  sem->state = interrupts_disable();
  --sem->count;

  return 0;
}

int sem_give(struct sem_t *sem)
{
  interrupts_enable (sem->state);
  ++sem->count;

  return 0;
}
