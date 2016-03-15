#ifndef __SEM__
#define __SEM__

struct sem_t
{
  int count;
  int state;
};

void sem_init(struct sem_t *sem, int count);
int sem_take(struct sem_t *sem);
int sem_give(struct sem_t *sem);

#endif /* __SEM__ */
