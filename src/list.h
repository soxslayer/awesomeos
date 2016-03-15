#ifndef __LIST__
#define __LIST__

struct list_t
{
  struct list_t *next;
  struct list_t *prev;
};

void list_init (struct list_t *list);
void list_add (struct list_t *list, struct list_t *node);
void list_remove (struct list_t *node);

#define list_head(l) ((l)->prev == NULL)
#define list_tail(l) ((l)->next == NULL)

#endif /* __LIST__ */
